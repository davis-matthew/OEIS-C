/*
 * A194525  Second coordinate of (4,7)-Lagrange pair for n.
 * https://oeis.org/A194525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*n - 4*floor((8*n + 6)/11)
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute floor((8*n + 6)/11) using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 num = (unsigned __int128)n * 8 + 6;
    unsigned __int128 div = num / 11;

    /* compute 3*n - 4*floor((8*n + 6)/11) */
    seq_t term = (seq_t)(3 * (seq_t)n - 4 * (seq_t)div);
    return term;
}
