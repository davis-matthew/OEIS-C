/*
 * A081552  Leading terms of rows in A081551.
 * https://oeis.org/A081552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 10^(n-1) + (n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 (offset of the sequence) */
    idx_t e = n - 1;               /* power of 10 needed   */
    seq_t term = pow10(e);         /* 10^(n-1) in seq_t  */
    term += (seq_t)e;              /* add (n-1)          */
    return term;
}
