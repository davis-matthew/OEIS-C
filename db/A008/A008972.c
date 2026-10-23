/*
 * A008972  n^2 raised to power n^2.
 * https://oeis.org/A008972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2)^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Avoid underflow from zero index */
    if (n == 0) return (seq_t)0;

    seq_t nn   = (seq_t)n;      /* n as seq_t */
    seq_t base = nn * nn;       /* n^2   */
    seq_t exp  = base;          /* n^2   */

    return big_pow(base, exp);  /* (n^2)^(n^2) */
}
