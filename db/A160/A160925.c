/*
 * A160925  a(n)= n - reversal(n+1).
 * https://oeis.org/A160925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - R(n+1) where R(x) is the decimal reversal of x
 * Range:     i8 n <= 61, i16 n <= 61, i32 n <= 61, i64 n <= 61, i128 n <= 61,
 *            big any n
 * Helpers:   rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rev.h"

seq_t a(idx_t n) {
    /* rev() returns an unsigned 64‑bit integer, suitable for n up to 2^64‑1 */
    idx_t revn = rev(n + 1);          /* R(n+1) */
    seq_t n_val = (seq_t)n;           /* convert index to signed seq_t */
    seq_t rev_val = (seq_t)revn;      /* convert reversal to signed seq_t */
    return n_val - rev_val;           /* a(n) = n - R(n+1) */
}
