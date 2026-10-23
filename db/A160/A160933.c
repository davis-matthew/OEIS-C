/*
 * A160933  a(n)= n - reversal(n-1) - reversal(n+1).
 * https://oeis.org/A160933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - rev(n-1) - rev(n+1)
 * Range:     i8 n <= 17, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"

seq_t a(idx_t n)
{
    /* Compute n in the target type to avoid unsigned wrap‑around. */
    seq_t nval = (seq_t)n;

    /* rev gives an idx_t; cast to seq_t for arithmetic. */
    seq_t rev_n_minus_1 = (seq_t)rev(n - 1);
    seq_t rev_n_plus_1  = (seq_t)rev(n + 1);

    return nval - rev_n_minus_1 - rev_n_plus_1;
}
