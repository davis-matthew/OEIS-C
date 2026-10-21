/*
 * A078987  Chebyshev U(n,x) polynomial evaluated at x=19.
 * https://oeis.org/A078987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 38, a(n) = 38*a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A078987 (Chebyshev U_n evaluated at x = 19) */
seq_t a(idx_t n) {
    /* handle base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)38;

    seq_t prev1 = (seq_t)1;   /* a(0) */
    seq_t prev2 = (seq_t)38;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)38 * prev2 - prev1;
        prev1 = prev2;
        prev2 = cur;
    }
    return prev2;
}
