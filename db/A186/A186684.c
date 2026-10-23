/*
 * A186684  Total number of positive integers below 10^n requiring 19 positive
 *          biquadrates in their representation as sum of biquadrates.
 * https://oeis.org/A186684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=1, a(n)=7 for n>=3
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the very small indices explicitly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    /* For all n >= 3 the value is constant */
    if (n >= 3) return (seq_t)7;
    /* Any unexpected index (n==0) returns 0 */
    return (seq_t)0;
}
