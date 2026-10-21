/*
 * A099129  Let T(n) be the n-th triangular number n*(n+1)/2; then a(n) = n-th
 *          iteration T(T(T(...(n)))).
 * https://oeis.org/A099129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = T^n(n) where T(x) = x*(x+1)/2 (n iterations, start with
 *            n)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;               /* start value */
    for (idx_t i = 0; i < n; ++i) {   /* apply T n times */
        x = (x * (x + (seq_t)1)) / (seq_t)2;
    }
    return x;
}
