/*
 * A076858  Number of geometries for group S_n under axioms A, 2T_1.
 * https://oeis.org/A076858
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = C(n+1,4) (with an exception at n = 5 giving 14)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;                     /* undefined below the offset */
    seq_t val = binom(n + 1, 4);              /* compute C(n+1,4) */
    if (n == 5) val -= 1;                    /* 5th entry is 14, not 15 */
    return val;
}
