/*
 * A099126  Number of orbits of the wreath product of S_n with S_n on n X n
 *          matrices over {0,1,2,3,4,5,6,7}.
 * https://oeis.org/A099126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(binomial(n + 7, n) + n - 1, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 100 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses a multiplicative binomial implementation that works for
   arbitrary precision seq_t when the first argument can be larger
   than 64 bits. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* Compute binomial(a, k) where a may be larger than 64 bits.
   k is assumed to be <= 64-bit and usually much smaller than a. */
static seq_t big_binomial(seq_t a, idx_t k) {
    if (k == 0) {
        return (seq_t)1;
    }

    /* If a is smaller than k, the binomial coefficient is zero.
       In our use case this does not occur, but we guard it anyway. */
    if (a < (seq_t)k) {
        return (seq_t)0;
    }

    seq_t result = (seq_t)1;
    /* Use the multiplicative formula:
       result = Π_{i=1..k} (a - k + i) / i  */
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator = a - (seq_t)(k - i);
        result = (result * numerator) / (seq_t)i;
    }
    return result;
}

/* Forward declaration of helper */
static seq_t big_binomial(seq_t a, idx_t k);

seq_t a(idx_t n) {
    /* Special case for a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* binomial(n + 7, n) == binomial(n + 7, 7) */
    seq_t first = binomial((idx_t)n + 7, n);

    /* Compute the upper parameter: first + n - 1 */
    seq_t upper = first + (seq_t)n - (seq_t)1;

    /* Compute binomial(upper, n) with a multiplicative algorithm */
    return big_binomial(upper, n);
}
