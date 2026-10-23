/*
 * A115274  a(n) = n + A115273(n), where A115273(n) = 0 for n = 1..3.
 * https://oeis.org/A115274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n/3)*(n mod 3) (n ≥ 1)
 * Range:     u8 n <= 154, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = (seq_t)(n / 3);
    seq_t r = (seq_t)(n % 3);
    return (seq_t)n + q * r;
}
