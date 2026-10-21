/*
 * A237710  Least prime p < n with pi(n-p) a square, or 0 if such a prime p
 *          does not exist.
 * https://oeis.org/A237710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p < n such that pi(n‑p) is a perfect square;
 *            returns 0 if no such prime exists
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, compute_pi, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, free */
#include <stdbool.h>   /* bool */

/* Utility prototypes required for this implementation */
#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit, &primes, &cnt) */
#include "../utils/compute_pi.h"          /* compute_pi(n) -> seq_t */
#include "../utils/floor_sqrt.h"          /* floor_sqrt(x) -> seq_t */

seq_t a(idx_t n)
{
    /* No prime < n for n ≤ 2 */
    if (n <= 2)
        return 0;

    /* 1. Build an array of all primes < n (i.e., ≤ n‑1). */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(n - 1, &primes, &primes_cnt);

    /* 2. Scan primes in ascending order to find the smallest one
       that satisfies the condition. */
    seq_t result = 0;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p64  = primes[i];
        idx_t    p    = (idx_t)p64;          /* p < n by construction */
        idx_t    m    = n - p;               /* n‑p ≥ 1 because p < n */

        seq_t pi_m = compute_pi(m);
        seq_t sq    = floor_sqrt(pi_m);

        /* Check if pi_m is a perfect square */
        if (sq * sq == pi_m) {
            result = (seq_t)p64;
            break;                           /* first such prime is the answer */
        }
    }

    /* 3. Clean up allocated memory */
    free(primes);

    return result;
}
