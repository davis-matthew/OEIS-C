/*
 * A140346  a(n) = binomial(n+8, 8)*5^n.
 * https://oeis.org/A140346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+8,8) * 5^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   binom, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    seq_t bin = binom(n + 8, 8);   /* compute C(n+8,8) */
    seq_t pow5 = power5(n);        /* compute 5^n   */
    return bin * pow5;             /* product           */
}
