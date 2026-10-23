/*
 * A172501  a(n) = binomial(n+8,8)*6^n.
 * https://oeis.org/A172501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8) * 6^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 38,
 *            big any n
 * Helpers:   binom, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    seq_t b = binom(n + 8, 8);          /* binomial coefficient */
    seq_t p = seq_pow((seq_t)6, n);     /* 6^n */
    return b * p;                       /* product */
}
