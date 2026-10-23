/*
 * A047243  Numbers that are congruent to {2, 3} mod 6.
 * https://oeis.org/A047243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 1 if n is odd * a(n) = 3*n - 3 if n is even *
 *            (equivalently a(n) = 3*n - 2 - (-1)^n)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for explicit types if needed */

seq_t a(idx_t n)
{
    /* offset is 1; n is guaranteed >=1 by the caller */
    seq_t n_seq = (seq_t)n;           /* cast index to seq_t */
    seq_t val   = n_seq * (seq_t)3;   /* 3 * n */

    /* subtract 1 for odd n, 3 for even */
    if (n & (idx_t)1)
        val = val - (seq_t)1;  /* odd */
    else
        val = val - (seq_t)3;  /* even */

    return val;
}
