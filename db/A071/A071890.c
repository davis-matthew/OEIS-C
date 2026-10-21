/*
 * A071890  a(n) = A071888(n)-n.
 * https://oeis.org/A071890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest m > n such that m is squarefree and shares
 *            a prime factor with n. It returns a(n) = m − n. The computation
 *            uses a simple smallest‑prime‑factor sieve for efficient
 *            square‑free tests
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd_u64.h"

/* Smallest prime factor table, up to spf_limit. */
static uint32_t *spf = NULL;
static size_t spf_limit = 0;

/* Euclidean GCD for 64‑bit values. */

/* Build spf table up to upper bound. */
static void ensure_spf(size_t limit) {
    if (limit <= spf_limit) return;

    uint32_t *new_spf = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_spf) abort();  /* out of memory */

    for (size_t i = 0; i <= limit; ++i) new_spf[i] = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == 0) new_spf[j] = (uint32_t)i;
            }
        }
    }

    if (spf) free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* Check whether x is squarefree, using the spf table. */
static bool is_squarefree(uint64_t x) {
    if (x <= 1) return true;
    while (x > 1) {
        uint32_t p = spf[(size_t)x];
        uint64_t p2 = (uint64_t)p * p;
        if (x % p2 == 0) return false;          /* p^2 divides x */
        while (x % p == 0) x /= p;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Special case for n == 1 (sequence definition). */
    if (n == 1) return (seq_t)1;

    /* Determine a safe bound for sieve work: up to roughly 4*n. */
    size_t bound = (size_t)n * 4 + 100;
    if (bound < 100) bound = 100;          /* guarantee tiny tables for small n */
    ensure_spf(bound);

    /* Factor n to get its distinct prime factors. */
    uint64_t nn = n;
    uint32_t primes[20];
    size_t primes_cnt = 0;
    while (nn > 1 && primes_cnt < sizeof(primes)/sizeof(primes[0])) {
        uint32_t p = spf[(size_t)nn];
        primes[primes_cnt++] = p;
        while (nn % p == 0) nn /= p;
    }

    uint64_t best_m = 0xFFFFFFFFFFFFFFFFULL;  /* essentially infinity */

    /* For each prime divisor of n, search for the smallest multiple with
       the required properties. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        /* Start with the smallest integer t such that p * t > n. */
        uint64_t t_start = n / p + 1;
        for (uint64_t t = t_start; ; ++t) {
            /* Skip if t shares the prime p (would give p^2). */
            if (t % p == 0) continue;
            /* Check squarefree condition. */
            if (!is_squarefree(t)) continue;
            uint64_t m = (uint64_t)p * t;
            if (m >= best_m) {
                /* Since t grows, m will only increase further; break early. */
                break;
            }
            best_m = m;
            /* Since further primes might yield an even smaller m, we continue. */
        }
    }

    /* best_m should now contain the desired value.  For safety, if none found
       (which should not happen except for n == 1 already handled), use n+1 */
    if (best_m == 0xFFFFFFFFFFFFFFFFULL) best_m = n + 1;

    /* Return a(n) = best_m - n */
    return (seq_t)(best_m - n);
}
