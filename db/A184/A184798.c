/*
 * A184798  Numbers m such that prime(m) is of the form floor(k*sqrt(3));
 *          complement of A184801.
 * https://oeis.org/A184798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A184798: Numbers m such that prime(m) == floor(k*√3)
   (complement of A184801).  The function scans the primes in
   increasing order, tests whether each prime equals floor(k*√3),
   and returns the index m of the n‑th such prime.  To avoid any
   floating‑point or library conflicts it uses only integer arithmetic. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- integer square root for unsigned __int128 (floor) ---- */
static unsigned long long isqrt128(unsigned __int128 x)
{
    unsigned __int128 bit = (unsigned __int128)1 << 126; /* highest 4‑power <= 2^128‑1 */
    unsigned __int128 res = 0;
    while (bit > x) bit >>= 2;
    while (bit) {
        unsigned __int128 tmp = res + bit;
        if (tmp <= x) {
            x -= tmp;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return (unsigned long long)res;
}

/* ---- test whether a prime p equals floor(k*√3) ---- */
static bool is_floor_of_sqrt3(uint64_t p)
{
    unsigned __int128 p2   = (unsigned __int128)p * p;
    unsigned __int128 low  = (p2 + 2) / 3;                     /* ceil(p²/3)   */
    unsigned __int128 up   = (unsigned __int128)(p + 1) * (p + 1) - 1;
    unsigned __int128 high = up / 3;                           /* floor(((p+1)²‑1)/3) */

    unsigned long long k_min = isqrt128(low);
    if ((unsigned __int128)k_min * k_min < low) k_min++;     /* round up */

    unsigned long long k_max = isqrt128(high);                /* round down */

    return k_min <= k_max;
}

/* ---- prime cache ------------------------------------------------ */
static uint64_t *prime_cache   = NULL;   /* array of primes found so far   */
static size_t   prime_cap     = 0;       /* current capacity of prime_cache */
static size_t   prime_cnt     = 0;       /* number of primes stored         */
static uint64_t next_candidate = 3;      /* next odd number to test         */

/* grow the prime cache when it fills up */
static int grow_prime_cache(void)
{
    size_t new_cap = prime_cap == 0 ? 16384 : prime_cap << 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
    if (!tmp) return -1;
    prime_cache = tmp;
    prime_cap   = new_cap;
    return 0;
}

/* add the next prime to prime_cache */
static void generate_next_prime(void)
{
    while (1) {
        uint64_t cand = next_candidate;
        next_candidate += 2;                     /* only odd numbers after 2   */

        /* trial division by known primes up to sqrt(cand) */
        bool ok = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t pr = prime_cache[i];
            if ((unsigned __int128)pr * pr > cand) break;
            if (cand % pr == 0) { ok = false; break; }
        }

        if (ok) {                                 /* prime found               */
            if (prime_cnt == prime_cap) grow_prime_cache();
            prime_cache[prime_cnt++] = cand;
            break;
        }
    }
}

/* ---- sequence function ------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state – persists between calls */
    static uint64_t matched = 0;          /* number of matches found so far */
    static uint64_t index   = 0;          /* current prime index (1‑based)   */

    /* first call: initialize prime cache with the first prime 2 */
    if (prime_cap == 0) {
        prime_cache = (uint64_t *)calloc(16384, sizeof(uint64_t));
        prime_cap   = 16384;
        prime_cache[prime_cnt++] = 2;      /* prime #1 */
    }

    /* handle prime 2 on first call only */
    if (matched == 0) {
        index = 1;                         /* prime #1 is 2 */
        if (is_floor_of_sqrt3(2)) {
            matched++;
            if (matched == (uint64_t)n) return (seq_t)index;
        }
    }

    /* generate primes until we have found the n‑th match */
    while (matched < (uint64_t)n) {
        generate_next_prime();              /* new prime becomes prime_cache[prime_cnt-1] */
        index++;
        uint64_t p = prime_cache[prime_cnt - 1];
        if (is_floor_of_sqrt3(p)) {
            matched++;
            if (matched == (uint64_t)n) return (seq_t)index;
        }
    }

    /* unreachable – safety return */
    return (seq_t)0;
}
