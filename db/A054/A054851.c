/*
 * A054851  a(n) = 2^(n-7)*binomial(n,7). Number of 7D hypercubes in an
 *          n-dimensional hypercube.
 * https://oeis.org/A054851
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-7)*binomial(n,7)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 21, u64 n <= 45, u128 n <= 100,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 7) {
        return (seq_t)0;          /* the sequence is defined only for n ≥ 7 */
    }

    seq_t bin = 1;
    idx_t start = n - 7;          /* n–7 + k will be used in the loop */

    /* Compute C(n,7) multiplicatively to avoid overflow */
    for (idx_t k = 1; k <= 7; ++k) {
        bin *= (seq_t)(start + k);
        bin /= (seq_t)k;         /* division is exact at each step */
    }

    seq_t two = pow2(n - 7);      /* 2^(n-7) */
    return bin * two;             /* a(n) = 2^(n-7) * C(n,7) */
}
