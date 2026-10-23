/*
 * A194511  Second coordinate of (2,5)-Lagrange pair for n.
 * https://oeis.org/A194511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - 2*floor((3*n + 4)/7)
 * Range:     i8 n <= 89, i16 n <= 89, i32 n <= 89, i64 n <= 89, i128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor((3*n + 4)/7) using unsigned arithmetic to avoid
       negative wrap‑around. */
    idx_t q = (3*n + 4) / 7;
    /* Convert to signed seq_t and compute the result. */
    seq_t result = (seq_t)n - (seq_t)(2*q);
    return result;
}
