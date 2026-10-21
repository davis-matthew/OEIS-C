/*
 * A089644  Numbers k such that 7 divides the numerator of B(2*k) where B(k) =
 *          the k-th Bernoulli number.
 * https://oeis.org/A089644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * ( n + floor((n-1)/2) )
 * Range:     u8 n <= 24, u16 n <= 6242, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence starts at a(1) = 7.  The formula below
       works for all n >= 1; for n = 0 the function returns 0
       simply because (n-1)/2 underflows in unsigned arithmetic.  */
    seq_t nn = (seq_t)n;          /* convert index to seq_t for arithmetic */
    seq_t k  = nn + (nn - 1) / 2; /* nth positive integer not divisible by 3 */
    return 7 * k;                 /* multiply by 7 to get the final term */
}
