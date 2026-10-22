/*
 * A005611  Number of Boolean functions realized by cascades of n gates.
 * https://oeis.org/A005611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1401*a(n-1) - 74360*a(n-2) + 375360*a(n-3) -
 *            302400*a(n-4), a(0) = 2, a(1) = 14, a(2) = 254, a(3) = 65534.
 *            Equivalently, with d(n) = a(n) - a(n-1): d(n) = 1400*d(n-1) -
 *            72960*d(n-2) + 302400*d(n-3), d(1) = 12, d(2) = 240, d(3) =
 *            65280
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t d1 = 12, d2 = 240, d3 = 65280;
    seq_t s;
    idx_t i;

    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)254;

    s = 65534;                       /* a(3) */
    for (i = 4; i <= n; i++) {
        /* add first so intermediates stay nonnegative */
        seq_t d4 = d3 * 1400 + d1 * 302400;
        d4 -= d2 * 72960;
        s += d4;
        d1 = d2;
        d2 = d3;
        d3 = d4;
    }
    return s;
}
