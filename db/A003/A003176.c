/*
 * A003176  Integer part of 24(2^n-1)/n.
 * https://oeis.org/A003176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(24*(2^n-1)/n); computed as 2^n-1 = q*n + r (r < n),
 *            so a(n) = 24*q + floor(24*r/n)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 65, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = 0;
    idx_t r = 0;
    idx_t i;
    for (i = 0; i < n; i++) {
        q = q * 2;
        r = 2 * r + 1;
        if (r >= n) {
            q += 1;
            r -= n;
        }
    }
    return q * 24 + (seq_t)((24 * r) / n);
}
