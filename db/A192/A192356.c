/*
 * A192356  Coefficients of x in the reduction of the polynomial p(n,x) =
 *          ((x+2)^n + (x-2)^n)/2 by x^2->x+2.
 * https://oeis.org/A192356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 12*a(n-2) + 2 for n ≥ 4 initial values: a(1) =
 *            0, a(2) = 1, a(3) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)1;
    seq_t prev2 = (seq_t)1;   /* a(2) */
    seq_t prev1 = (seq_t)1;   /* a(3) */
    for (idx_t k = 4; k <= n; ++k) {
        seq_t cur = prev1 + (seq_t)12 * prev2 + (seq_t)2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
