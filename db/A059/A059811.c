/*
 * A059811  Let g_n be the ball packing n-width for the manifold torus X
 *          interval; sequence gives numerator of (g_n/Pi)^2.
 * https://oeis.org/A059811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1 + 3*[(n≥5)] + 5*[(n≥7)] + 27*[(n≥8)] - 35*[(n≥9)]
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 0;
    if (n >= 1) v += 1;
    if (n >= 5) v += 3;
    if (n >= 7) v += 5;
    if (n >= 8) v += 27;
    if (n >= 9) v -= 35;
    return v;
}
