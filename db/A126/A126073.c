/*
 * A126073  Sum of numbers <= n which are multiples of 3 or 5 but not 15.
 * https://oeis.org/A126073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = SN(n,3)+SN(n,5)-2*SN(n,15), where SN(n,d) = d *
 *            floor(n/d) * (floor(n/d) + 1) / 2
 * Range:     u8 n <= 35, u16 n <= 572, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the floor divisions */
    idx_t m3  = n / 3;     /* floor(n/3)  */
    idx_t m5  = n / 5;     /* floor(n/5)  */
    idx_t m15 = n / 15;    /* floor(n/15) */

    /* Sum of multiples of 3, 5, and 15 up to n */
    seq_t sum3  = (seq_t)3 * (seq_t)m3 * ((seq_t)m3 + 1) / 2;
    seq_t sum5  = (seq_t)5 * (seq_t)m5 * ((seq_t)m5 + 1) / 2;
    seq_t sum15 = (seq_t)15 * (seq_t)m15 * ((seq_t)m15 + 1) / 2;

    /* Return the inclusion–exclusion result */
    return sum3 + sum5 - (seq_t)2 * sum15;
}
