/*
 * A120950  a(n) = 3^n+5^n-2n.
 * https://oeis.org/A120950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + 5^n - 2n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 3^n and 5^n */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)n);
    seq_t pow5 = big_pow((seq_t)5, (seq_t)n);

    /* a(n) = 3^n + 5^n - 2n */
    seq_t result = pow3 + pow5;
    result -= (seq_t)(2ULL * n);
    return result;
}
