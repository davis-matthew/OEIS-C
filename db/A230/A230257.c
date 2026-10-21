/*
 * A230257  The number of multinomial coefficients over partitions with value
 *          equal to 9.
 * https://oeis.org/A230257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/8) + floor((n-1)/9) - floor(n/9)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since n ≥ 1, n-1 is safe */
    idx_t n_minus_one = n - 1;
    seq_t term1 = (seq_t)(n_minus_one / 8);
    seq_t term2 = (seq_t)(n_minus_one / 9);
    seq_t term3 = (seq_t)(n / 9);
    return term1 + term2 - term3;
}
