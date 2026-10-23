/*
 * A040966  Continued fraction for sqrt(998).
 * https://oeis.org/A040966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = 31 for n = 0, otherwise the partial quotients of the
 *            continued fraction of sqrt(998), which repeat with period 18:
 *            [1, 1, 2, 4, 8, 1, 3, 1, 30, 1, 3, 1, 8, 4, 2, 1, 1, 62]
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)31;
    static const seq_t period[18] = { 1, 1, 2, 4, 8, 1, 3, 1, 30, 1, 3, 1, 8, 4, 2, 1, 1, 62 };
    idx_t mod = (n - 1) % 18;
    return period[(int)mod];
}
