/*
 * A184112  a(n) = n + ceiling(n^2/16); complement of A184111.
 * https://oeis.org/A184112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ceil(n^2/16)
 * Range:     u8 n <= 15, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A184112 as a direct formula. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) is undefined; return 0 for safety */
    seq_t nn   = (seq_t)n;
    seq_t sq   = nn * nn;          /* compute n^2 in the precision of seq_t */
    seq_t add  = (sq + (seq_t)15) / (seq_t)16;  /* ceil(n^2/16) */
    return nn + add;
}
