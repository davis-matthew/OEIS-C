/*
 * A213502  Number of (w,x,y) with all terms in {0,...,n} and x != min(|w-x|,
 *          |x-y|).
 * https://oeis.org/A213502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(2*n^2+4*n+1) - (-1)^n + 1)/4
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute 2*n*(2*n^2 + 4*n + 1) */
    seq_t term = 2 * nn * (2 * nn * nn + 4 * nn + (seq_t)1);
    /* For odd n, (-1)^n = -1, so subtracting (-1)^n adds 1.
       Hence the numerator becomes term + 2. For even n, numerator is term. */
    if ((n & 1u) != 0u)
        term += (seq_t)2;
    return term / (seq_t)4;
}
