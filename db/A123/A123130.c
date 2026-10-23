/*
 * A123130  a(n) = 2^(n-1)*((2*n)!/n!) * Integral_{t=0..Pi/3} sin(t)^(2*n-1)
 *          dt.
 * https://oeis.org/A123130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(2) = 5 a(n) = (14*n-17) * a(n-1) - 24*(n-2)*(2*n-3) *
 *            a(n-2) for n >= 3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1 – n=0 never used */
    if (n == 1) return 1;
    if (n == 2) return 5;

    seq_t a_prev2 = 1;             /* a(1) */
    seq_t a_prev1 = 5;             /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t coeff1 = (seq_t)(14*i - 17);
        seq_t coeff2 = (seq_t)24 * (seq_t)(i-2) * (seq_t)(2*i - 3);
        seq_t term1 = coeff1 * a_prev1;
        seq_t term2 = coeff2 * a_prev2;
        seq_t a_cur = term1 - term2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }
    return a_prev1;
}
