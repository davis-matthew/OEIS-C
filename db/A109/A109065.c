/*
 * A109065  Numerator of the fraction due in month n of the total interest for
 *          a one-year installment loan based on the Rule of 78s (each
 *          denominator is 78).
 * https://oeis.org/A109065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, frac, full, nonn
 * Formula:   a(n) = 12 - ((n-1) mod 12)
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n-1) modulo 12 using seq_t arithmetic */
    seq_t mod = ((seq_t)(n - 1)) % (seq_t)12;
    /* Subtract the remainder from 12 to get the numerator */
    return (seq_t)12 - mod;
}
