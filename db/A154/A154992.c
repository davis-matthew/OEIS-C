/*
 * A154992  A048473 prefixed by two zeros.
 * https://oeis.org/A154992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 2*3^(n-2) - 1 for n >= 2; a(0) = a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;

    seq_t v = (seq_t)1;          /* 3^0 */
    idx_t e = n - 2;             /* exponent of 3 */

    for (idx_t i = 0; i < e; ++i)
        v = v * (seq_t)3;        /* multiply by 3 each step */

    /* a(n) = 2*3^(n-2) - 1 */
    v = v * (seq_t)2 - (seq_t)1;
    return v;
}
