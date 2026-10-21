/*
 * A211547  The squares n^2, n >= 0, each one written three times.
 * https://oeis.org/A211547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3)^2 Every value is computed as the square of the
 *            integer division of n by 3
 * Range:     u8 n <= 47, u16 n <= 767, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)(n / 3);
    return k * k;
}
