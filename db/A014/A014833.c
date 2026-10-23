/*
 * A014833  a(n) = 2^n - n*(n+1)/2.
 * https://oeis.org/A014833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n*(n+1)/2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow = pow * ((seq_t)2);           /* compute 2^n */

    seq_t nn  = (seq_t)n;
    seq_t tri = nn * (nn + 1) / 2;        /* compute n*(n+1)/2 */

    return pow - tri;                    /* final result */
}
