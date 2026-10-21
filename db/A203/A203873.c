/*
 * A203873  Number of (n+1)X4 0..3 arrays with no 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A203873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 486*49^(n-1) + 1
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the provided big_pow utility for arbitrary-precision exponentiation. */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* defined for n ≥ 1, below offset ignored */
    seq_t exponent = (seq_t)(n - 1);
    seq_t res = big_pow((seq_t)49, exponent);   /* compute 49^(n-1) */
    res *= (seq_t)486;                          /* multiply by 486 */
    res += (seq_t)1;                            /* add 1 */
    return res;
}
