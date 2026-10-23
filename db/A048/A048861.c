/*
 * A048861  a(n) = n^n - 1.
 * https://oeis.org/A048861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n - 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sn  = (seq_t)n;          /* convert index to seq_t */
    seq_t p   = big_pow(sn, sn);   /* compute n^n exactly */
    return p - (seq_t)1;           /* subtract 1 */
}
