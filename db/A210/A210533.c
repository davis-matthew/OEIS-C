/*
 * A210533  Number of ways to write 2n = x+y (x,y>0) with x-1 and x+1 both
 *          prime, and x and x^3+y^3 both practical.
 * https://oeis.org/A210533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions of x + y = 2n (x,y > 0) with x‑1 and
 *            x+1 both prime, x practical, and x^3 + y^3 practical
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------------------------------------------------------------------- */
/* Prime sieve up to 3 000 000 – enough for all factorisations          */
#define PRIME_LIMIT 3000000
#define MAX_PRIME_LIST 250000        /* > prime count up to PRIME_LIMIT */
static uint8_t sieve[PRIME_LIMIT + 1];
static uint32_t prime_list[MAX_PRIME_LIST];
static size_t prime_cnt;

/* Practicality cache for numbers up to 2*MAX_N (used for small x).     */
#define MAX_N 20000
static bool practical_small[MAX_N + 1];

/* ---------------------------------------------------------------------- */
static void init_primes(void) {
    static bool done = false;
    if (done) return;
    done = true;

    /* simple sieve */
    for (int i = 0; i <= PRIME_LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (int i = 2; (long long)i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (int j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;

    /* build list of primes */
    prime_cnt = 0;
    for (int i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) prime_list[prime_cnt++] = (uint32_t)i;

    /* cache practicality for numbers up to MAX_N */
    practical_small[0] = practical_small[1] = true;
    for (uint32_t n = 2; n <= MAX_N; ++n)
        practical_small[n] = true;   /* placeholder, will be set below */
    /* compute practicality of each n */
    for (uint32_t n = 2; n <= MAX_N; ++n) {
        uint64_t rem = n;
        unsigned __int128 sigma = 1;
        bool first = true;
        for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] *
             (uint64_t)prime_list[i] <= rem; ++i) {
            uint32_t p = prime_list[i];
            if (rem % p == 0) {
                int exp = 0;
                while (rem % p == 0) {
                    rem /= p;
                    ++exp;
                }
                if (first) {
                    if (p != 2) { practical_small[n] = false; break; }
                    first = false;
                } else {
                    if ((unsigned __int128)p > sigma + 1) { practical_small[n] = false; break; }
                }
                unsigned __int128 pow = 1;
                for (int k = 0; k < exp + 1; ++k) pow *= p;
                unsigned __int128 sigma_p = (pow - 1) / (p - 1);
                sigma *= sigma_p;
            }
        }
        if (!practical_small[n]) continue;
        if (rem > 1) {                     /* a remaining prime factor */
            uint64_t p = rem;
            if (first) {
                if (p != 2) { practical_small[n] = false; continue; }
                first = false;
            } else {
                if ((unsigned __int128)p > sigma + 1) { practical_small[n] = false; continue; }
            }
            unsigned __int128 pow = (unsigned __int128)p * (unsigned __int128)p;
            unsigned __int128 sigma_p = (pow - 1) / (p - 1);
            sigma *= sigma_p;
        }
        practical_small[n] = true;
    }
}

/* ---------------------------------------------------------------------- */
/* Test whether a 64‑bit number is practical (general case).              */
static bool is_practical_uint64(uint64_t n) {
    if (n <= 1) return true;
    unsigned __int128 sigma = 1;
    uint64_t rem = n;
    bool first = true;
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] *
         (uint64_t)prime_list[i] <= rem; ++i) {
        uint32_t p = prime_list[i];
        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            if (first) {
                if (p != 2) return false;
                first = false;
            } else {
                if ((unsigned __int128)p > sigma + 1) return false;
            }
            unsigned __int128 pow = 1;
            for (int k = 0; k < exp + 1; ++k) pow *= p;
            unsigned __int128 sigma_p = (pow - 1) / (p - 1);
            sigma *= sigma_p;
        }
    }
    if (rem > 1) { /* remaining prime factor */
        uint64_t p = rem;
        if (first) {
            if (p != 2) return false;
            first = false;
        } else {
            if ((unsigned __int128)p > sigma + 1) return false;
        }
        unsigned __int128 pow = (unsigned __int128)p * (unsigned __int128)p;
        unsigned __int128 sigma_p = (pow - 1) / (p - 1);
        sigma *= sigma_p;
    }
    return true;
}

/* ---------------------------------------------------------------------- */
/* Main sequence function                                               */
seq_t a(idx_t n) {
    init_primes();            /* one‑time prime and practical cache */

    uint64_t two_n = (uint64_t)n * 2ULL;
    seq_t count = 0;

    for (uint64_t x = 1; x < two_n; ++x) {
        uint64_t y = two_n - x;
        /* x-1 and x+1 must be prime */
        if (!sieve[x - 1] || !sieve[x + 1]) continue;

        /* x must be practical */
        if (!practical_small[(uint32_t)x]) continue;

        /* sum = x^3 + y^3 must be practical */
        unsigned __int128 x3 = (unsigned __int128)x * x * x;
        unsigned __int128 y3 = (unsigned __int128)y * y * y;
        unsigned __int128 sum128 = x3 + y3;
        uint64_t sum = (uint64_t)sum128;
        if (!is_practical_uint64(sum)) continue;

        ++count;
    }
    return (seq_t)count;
}
