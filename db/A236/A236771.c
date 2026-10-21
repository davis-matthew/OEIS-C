/*
 * A236771  a(n) = n + floor(n/2 + n^2/3).
 * https://oeis.org/A236771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n/2) + floor(n^2/3)
 * Range:     u8 n <= 25, u16 n <= 441, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently a(n) = n + floor(n/2 + n^2/3)) */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn + nn / 2 + (nn * nn) / 3;
}
