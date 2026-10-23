/*
 * A010053  a(n) = 4^n*(2*n+1)!*(n!)^2/(n+1).
 * https://oeis.org/A010053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * (2*n+1)! * (n!)^2 / (n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    seq_t res = (seq_t)1;

    /* Compute 4^n */
    for (idx_t i = 0; i < n; ++i) {
        res *= (seq_t)4;
    }

    /* Multiply by (2n+1)! */
    for (idx_t i = 1; i <= 2 * n + 1; ++i) {
        res *= (seq_t)i;
    }

    /* Compute n! and square it */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }
    res *= fact;
    res *= fact;

    /* Divide by (n+1) */
    res /= (seq_t)(n + 1);

    return res;
}
