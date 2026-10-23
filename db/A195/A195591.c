/*
 * A195591  Number of ways to place 3n nonattacking kings on a vertical
 *          cylinder 6 X 2n.
 * https://oeis.org/A195591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (31*n - 65)*2^n + 18*n + 66 (valid for n >= 1, following
 *            the OEIS offset)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 22, u64 n <= 53, u128 n <= 116,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used, but harmless */

    seq_t n_seq = (seq_t)n;         /* promote index to seq_t */
    seq_t term = 31 * n_seq;        /* 31*n */
    term -= (seq_t)65;              /* (31*n - 65) */
    term *= pow2(n);                /* * 2^n */
    term += (seq_t)18 * n_seq;      /* + 18*n */
    term += (seq_t)66;              /* + 66 */
    return term;
}
