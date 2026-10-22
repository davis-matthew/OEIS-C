/*
 * A000212  a(n) = floor(n^2/3).
 * https://oeis.org/A000212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2/3); with n = 3q + r (0 <= r < 3), a(n) = q*(3q
 *            + 2r) + floor(r^2/3)
 * Range:     u8 n <= 27, u16 n <= 443, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 3;
    idx_t r = n % 3;
    seq_t Q = (seq_t)q;
    seq_t res = Q * (Q * 3 + (seq_t)(2 * r));
    if (r == 2)
        res += 1;
    return res;
}
