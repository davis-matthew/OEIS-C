/*
 * A017107  a(n) = (8*n+3)^7.
 * https://oeis.org/A017107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n+3)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 70, u128 n <= 10000,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t base = 8 * (seq_t)n + (seq_t)3;  /* (8*n+3) */
    seq_t res  = (seq_t)1;                /* accumulator for base^7 */

    for (int i = 0; i < 7; ++i)
    {
        res = seq_mul(res, base);         /* multiply using the provided helper */
    }
    return res;
}
