/*
 * A112332  a(n) = Product_{k=0..n-1} k!*binomial(2k,k).
 * https://oeis.org/A112332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=0..n-1} k!*C(2k,k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;   /* product so far */
    seq_t fact   = 1;   /* k! for current k  */
    seq_t binom  = 1;   /* C(2k,k) for current k */

    for (idx_t k = 1; k < n; ++k) {
        /* update k! */
        fact = fact * (seq_t)k;

        /* update central binomial coefficient:
           C(2k,k) = C(2(k-1),k-1) * (2k)(2k-1)/(k*k) */
        seq_t two_k = (seq_t)k * 2;
        seq_t mult  = binom * two_k * (two_k - 1);
        seq_t denom = (seq_t)k * (seq_t)k;
        binom = mult / denom;   /* exact division */

        /* term = k! * C(2k,k) */
        seq_t term = fact * binom;
        result = result * term;
    }
    return result;
}
