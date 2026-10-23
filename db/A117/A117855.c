/*
 * A117855  Number of nonzero palindromes of length n (in base 3).
 * https://oeis.org/A117855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2*3^floor((n-1)/2)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Guard against n = 0 if called mistakenly */
    if (n == 0) return (seq_t)0;

    /* k = floor((n-1)/2) */
    idx_t k = (n - 1) >> 1;

    /* 3^k */
    seq_t p = pow3(k);

    /* 2 * 3^k */
    return p * (seq_t)2;
}
