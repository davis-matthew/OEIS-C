/*
 * A213399  Number of (w,x,y) with all terms in {0,...,n} and max(|w-x|,|x-y|)
 *          = x.
 * https://oeis.org/A213399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n^2 + 2*(-1)^n*n + 8*n + (-1)^n + 3)/4
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent piecewise form for integer arithmetic:
   if n is even: a(n) = (4*n^2 + 5*n + 2)/2
   if n is odd : a(n) = (4*n^2 + 3*n + 1)/2
*/
seq_t a(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t result;
    if ((n & 1) == 0) {
        result = ((seq_t)4 * n_sq + (seq_t)5 * (seq_t)n + 2) / 2;
    } else {
        result = ((seq_t)4 * n_sq + (seq_t)3 * (seq_t)n + 1) / 2;
    }
    return result;
}
