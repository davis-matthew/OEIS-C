/*
 * A159868  Numerator of Hermite(n, 4/23).
 * https://oeis.org/A159868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=8, for n>=2: a(n) = 8*a(n-1) - 1058*(n-1)*a(n-2)
 *            (derived from the e.g.f. exp(8x-529x²) )
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    if (n == (idx_t)1) {
        return (seq_t)8;
    }
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)8;   /* a(1) */
    seq_t a_curr;
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        /* factor = 1058*(i-1) */
        seq_t factor = (seq_t)1058 * (seq_t)(i - (idx_t)1);
        a_curr = (seq_t)8 * a_prev1 - factor * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_curr;
}
