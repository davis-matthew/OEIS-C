/*
 * A229135  a(n) = n * (2 + 2^(2*n - 1)).
 * https://oeis.org/A229135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2 + 2^(2*n-1))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For n = 0 the formula would involve 2^(-1), so handle it separately. */
    if (n == 0)
        return (seq_t)0;

    /* Compute 2^(2*n-1) */
    idx_t two_n_minus_one_idx = 2 * n - 1;          /* idx_t is unsigned 64‑bit */
    seq_t two_n_minus_one = (seq_t)two_n_minus_one_idx;

    seq_t pow2 = big_pow((seq_t)2, two_n_minus_one);

    /* a(n) = n * (2 + pow2) */
    seq_t sum = (seq_t)2 + pow2;
    seq_t result = (seq_t)n * sum;

    return result;
}
