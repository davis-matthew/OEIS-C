/*
 * A203872  Number of (n+1)X3 0..3 arrays with no 2X2 subblock having equal
 *          diagonal elements or equal antidiagonal elements, and new values
 *          0..3 introduced in row major order.
 * https://oeis.org/A203872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 54*21^(n-1) + 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* Return the nth term of OEIS sequence A203872.
 * The offset is 1: a(1) = 55, a(2) = 1135, etc. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* undefined for offset 1, return 0 for safety */
        return (seq_t)0;

    /* exponent (n-1) as a seq_t */
    seq_t exp = (seq_t)(n - (idx_t)1);

    /* compute 21^(n-1) */
    seq_t pow = big_pow((seq_t)21, exp);

    /* multiply by 54 and add 1 */
    seq_t res = pow * (seq_t)54;
    res += (seq_t)1;
    return res;
}
