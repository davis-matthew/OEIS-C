/*
 * A187318  a(n) = floor(9*n/5).
 * https://oeis.org/A187318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(4*n/5)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 5;
    idx_t r = n % 5;
    idx_t res = n + (q * 4 + (r * 4) / 5);
    return (seq_t)res;
}
