/*
 * A097774  Chebyshev U(n,x) polynomial evaluated at x=393=2*14^2+1.
 * https://oeis.org/A097774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=786, a(n)=2*393*a(n-1)-a(n-2) for n≥2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)786;

    seq_t prev2 = (seq_t)1;    /* a(n-2) */
    seq_t prev1 = (seq_t)786;  /* a(n-1) */
    const seq_t coef = (seq_t)786;  /* 2*393 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t curr = (prev1 * coef) - prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
