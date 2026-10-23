/*
 * A008363  a(n) = floor(n/5)*ceiling(n/5).
 * https://oeis.org/A008363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/5)*ceil(n/5)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 5;
    idx_t rem = n % 5;
    idx_t c = q + (rem ? 1 : 0);
    return (seq_t)q * (seq_t)c;
}
