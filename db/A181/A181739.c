/*
 * A181739  Smallest semiprime with internal digits = n; or 0 if no such
 *          number exists.
 * https://oeis.org/A181739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest semiprime that contains decimal n as a
 *            proper substring, obtained by inserting a digit (1…9) on both
 *            sides of n. If no such number exists, a(n) = 0. The function
 *            searches the 81 possible candidates and tests them by trial
 *            division. A static list of primes up to 40000 is built...
 * Range:     u8 n <= 9, u16 n <= 999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PRIME_CAP 5000
#define LIMIT     40000

/* prime table */
static int primes[PRIME_CAP];
static size_t primes_cnt = 0;
static bool primes_init = false;

/* build primes up to LIMIT (once) */
static void init_primes(void)
{
    if (primes_init) return;
    static unsigned char is_comp[LIMIT + 1];
    for (int i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt < PRIME_CAP) primes[primes_cnt++] = i;
            if ((int64_t)i * i <= LIMIT) {
                for (int j = i * i; j <= LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    primes_init = true;
}

/* number of decimal digits of a uint64_t */
static inline int digit_len_uint64(uint64_t x)
{
    int len = 0;
    do {
        ++len;
        x /= 10;
    } while (x);
    return len;
}

/* test whether n is a semiprime (product of exactly two primes) */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;          /* 2*2 is the smallest semiprime */
    uint64_t p = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        int pr = primes[i];
        if ((uint64_t)pr * (uint64_t)pr > n) break;
        if (n % pr == 0) { p = pr; break; }
    }
    if (p == 0) return false;          /* n is prime */
    uint64_t q = n / p;
    if (q == 1) return false;          /* should not happen */
    for (size_t i = 0; i < primes_cnt; ++i) {
        int pr = primes[i];
        if ((uint64_t)pr * (uint64_t)pr > q) break;
        if (q % pr == 0) return false; /* q composite */
    }
    return true;
}

/* main sequence function */
seq_t a(idx_t n)
{
    init_primes();                     /* ensure primes are ready */

    uint64_t un = (uint64_t)n;
    int len = digit_len_uint64(un);

    /* 10^(len+1)  */
    uint64_t pow10 = 1;
    for (int i = 0; i < len + 1; ++i) pow10 *= 10ULL;

    uint64_t best = 0;
    for (int d1 = 1; d1 <= 9; ++d1) {
        for (int d2 = 1; d2 <= 9; ++d2) {
            uint64_t cand = (uint64_t)d1 * pow10 + un * 10ULL + (uint64_t)d2;
            if (is_semiprime(cand) && (best == 0 || cand < best))
                best = cand;
        }
    }
    return (seq_t)best;
}
