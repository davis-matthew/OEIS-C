/*
 * A130489  a(n) = Sum_{k=0..n} (k mod 11) (Partial sums of A010880).
 * https://oeis.org/A130489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 55*(n/11) + ((n%11)*((n%11)+1))/2
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the quotient and remainder of n divided by 11 */
    seq_t q = (seq_t)(n / 11);
    seq_t r = (seq_t)(n % 11);

    /* Safe integer arithmetic: use seq_t for all operations */
    seq_t part = r * (r + (seq_t)1) / (seq_t)2;
    return (seq_t)55 * q + part;
}
