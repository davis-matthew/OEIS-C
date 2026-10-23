/*
 * A115273  a(n) = floor(n/3)*(n mod 3).
 * https://oeis.org/A115273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3) * (n mod 3)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t q = n / 3;
    idx_t r = n - q * 3;   /* r = n % 3 */
    return (seq_t)q * (seq_t)r;
}
