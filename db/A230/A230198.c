/*
 * A230198  The number of multinomial coefficients over partitions with value
 *          equal to 8.
 * https://oeis.org/A230198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/7) + floor((n-1)/8) - floor(n/8)
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n<1, safe guard */
    seq_t n_minus_1 = (seq_t)n - 1;
    seq_t term1 = n_minus_1 / 7;          /* floor((n-1)/7) */
    seq_t term2 = n_minus_1 / 8;          /* floor((n-1)/8) */
    seq_t term3 = (seq_t)n / 8;           /* floor(n/8)     */
    return term1 + term2 - term3;        /* resulting count */
}
