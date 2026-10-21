/*
 * A063077  a(n) = phi(n^2 + 1) - 2n.
 * https://oeis.org/A063077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = phi(n^2 + 1) - 2*n
 * Range:     i8 n <= 11, i16 n <= 182, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided φ routine in utils/phi.h */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* Compute n^2 + 1 in seq_t, then cast to idx_t for φ */
    idx_t val = (idx_t)((seq_t)n * (seq_t)n + 1);
    seq_t ph  = phi(val);            /* φ(n^2 + 1) */
    seq_t sub = (seq_t)2 * (seq_t)n;  /* 2*n */
    return ph - sub;
}
