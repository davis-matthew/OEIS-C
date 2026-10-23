/*
 * A100503  Bisection of A000125.
 * https://oeis.org/A100503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 5*n + 3)/3
 * Range:     u8 n <= 5, u16 n <= 36, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow for 64‑bit seq_t.
       The result always fits in seq_t for the tested range. */
    unsigned __int128 nn = n;
    unsigned __int128 v = 4 * nn * nn * nn + 5 * nn + 3;
    v /= 3;
    return (seq_t)v;
}
