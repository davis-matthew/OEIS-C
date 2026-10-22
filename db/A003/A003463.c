/*
 * A003463  a(n) = (5^n - 1)/4.
 * https://oeis.org/A003463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) + 1, a(0) = 0 (equivalently a(n) = (5^n - 1)/4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++)
        r = r * 5 + 1;
    return r;
}
