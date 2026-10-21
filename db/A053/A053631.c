/*
 * A053631  Pythagorean spiral: a(n-1)+1, a(n) and a(n)+1 are the sides of a
 *          right triangle (a primitive Pythagorean triangle).
 * https://oeis.org/A053631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; for n >= 2: a(n) = a(n-1) * (a(n-1) + 2) / 2
 *            (equivalently a(n) = ((a(n-1) + 1)^2 - 1) / 2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)2;           /* a(1) */
    if (n <= (idx_t)1)
        return val;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(n) = a(n-1) * (a(n-1) + 2) / 2 */
        val = val * (val + (seq_t)2);
        val = val / (seq_t)2;
    }
    return val;
}
