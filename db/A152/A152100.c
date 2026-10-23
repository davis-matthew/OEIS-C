/*
 * A152100  Expansion of g.f. 1 - 2*x*(-7 - 10*x + x^2)/(x - 1)^4.
 * https://oeis.org/A152100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 2*n*(8*n^2 + 12*n + 1)/3 for n > 0
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 673, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t nn = (seq_t)n;
    /* Compute 8*n^2 + 12*n + 1 safely */
    seq_t t1 = (seq_t)8 * nn * nn;
    seq_t t2 = (seq_t)12 * nn;
    seq_t t  = t1 + t2 + (seq_t)1;
    /* Numerator 2*n*t, then divide by 3 */
    seq_t numer = (seq_t)2 * nn * t;
    return numer / (seq_t)3;
}
