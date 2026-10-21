/*
 * A214154  Number of ways to represent 2n as the sum of two distinct k-almost
 *          primes: #{m<n | A001222(m)=A001222(2n-m)}.
 * https://oeis.org/A214154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): number of representations of 2n as the sum of two distinct
   numbers with the same number of prime divisors (counted with multiplicity). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_spf.h"

seq_t a(idx_t n)
{
    /* Return 0 for n < 1 to keep behavior well defined. */
    if (n < 1)
        return 0;

    idx_t limit = n * 2ULL;       /* 2n */

    /* Build smallest prime factor table up to 'limit'. */
    idx_t *spf = NULL;
    build_spf(limit, &spf);

    /* Allocate an array to hold Ω(i) for 0 ≤ i ≤ limit. */
    uint8_t *omega = (uint8_t *)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    if (!omega) {
        free(spf);
        return 0;                 /* Allocation failure */
    }

    /* Base cases: Ω(0) = 0, Ω(1) = 0. */
    omega[0] = 0;
    if (limit >= 1)
        omega[1] = 0;

    /* Compute Ω(i) using the SPF array. */
    for (size_t i = 2; i <= (size_t)limit; ++i) {
        idx_t p = spf[i];
        omega[i] = omega[(size_t)(i / p)] + 1;
    }

    /* Count pairs (m, 2n−m) with m < n and Ω(m) = Ω(2n−m). */
    seq_t count = 0;
    for (idx_t m = 1; m < n; ++m) {
        idx_t other = limit - m;
        if (omega[(size_t)m] == omega[(size_t)other])
            ++count;
    }

    free(omega);
    free(spf);
    return count;
}
