/*
 * A074799  a(n) = numerator( (4*n+1)*(Product_{i=1..n}
 *          (2*i-1)/Product_{i=1..n} 2*i)^5 ).
 * https://oeis.org/A074799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = numerator( (4*n+1)*(binomial(2*n,n)/4^n)^5 )
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute central binomial coefficient C(2n,n) n times,
   raise it to the 5th power, multiply by (4n+1), then divide out all
   factors of 2 that cancel with the denominator (2^10n).  The power
   of 2 in C(2n,n) equals popcount(n), so the total number of 2’s in
   the numerator is 5·popcount(n).  Hence we can simply shift right
   by 5·popcount(n) to get the numerator of the reduced fraction. */
seq_t a(idx_t n)
{
    /* compute C(2n,n) multiplicatively: C(2n,n)=Π_{i=1..n} (n+i)/i */
    seq_t C = 1;
    for (idx_t i = 1; i <= n; ++i) {
        C = (C * (seq_t)(n + i)) / (seq_t)i;
    }

    /* compute C^5 */
    seq_t pow5 = C;
    pow5 = pow5 * C; /* C^2 */
    pow5 = pow5 * C; /* C^3 */
    pow5 = pow5 * C; /* C^4 */
    pow5 = pow5 * C; /* C^5 */

    /* multiply by (4n+1) */
    seq_t num = (seq_t)(4 * n + 1) * pow5;

    /* shift right by 5*popcount(n) to remove power of 2 */
    unsigned int shift = 5 * __builtin_popcountll(n);
    seq_t res = num >> shift;

    return res;
}
