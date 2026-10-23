/*
 * A159743  If an array is made of columns of -nacci sequences, fibo-, tribo-
 *          etc. all starting w. 1,1,2 etc, the NW to SE diagonals can be
 *          extended by computation. The above is diagonal 7. See A159741 for
 *          details.
 * https://oeis.org/A159743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 21, a(2) = 81, and for n ≥ 3 a(n) = 2^(n+5) – 48
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Derived from the OEIS entry A159743.)                 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)81;
    /* For n ≥ 3 use the closed‑form 2^(n+5) – 48. */
    return pow2(n + 5) - (seq_t)48;
}
