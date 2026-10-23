/*
 * A134018  Let P(A) be the power set of an n-element set A. Then a(n) = the
 *          number of pairs of elements {x,y} of P(A) for which either 0) x
 *          and y are disjoint and for which either x is a subset of y or y is
 *          a subset of x, or 1) x and y are intersecting but for which x is
 *          not a subset of y and y is not a sub...
 * https://oeis.org/A134018
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2)(4^n - 3^(n+1) + 5*2^n - 3)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t pow2 = 1, pow3 = 1, pow4 = 1;
    idx_t i;
    for (i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
        pow3 *= (seq_t)3;
        pow4 *= (seq_t)4;
    }
    /* pow3 is now 3^n, compute 3^(n+1) */
    seq_t pow3_nplus1 = pow3 * (seq_t)3;
    seq_t numerator = pow4 - pow3_nplus1 + (seq_t)5 * pow2 - (seq_t)3;
    seq_t result = numerator / (seq_t)2;
    return result;
}
