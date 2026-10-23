/*
 * A015476  q-Fibonacci numbers for q=5, scaling a(n-1).
 * https://oeis.org/A015476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n-1) * a(n-1) + a(n-2) Initial values: a(0) = 0, a(1)
 *            = 1 Recurrence applied for n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_prev2 = (seq_t)0;            /* a(0) */
    seq_t a_prev1 = (seq_t)1;            /* a(1) */
    seq_t pow5    = (seq_t)5;            /* 5^(1) for the step n = 2 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t a_cur = pow5 * a_prev1 + a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
        pow5 *= (seq_t)5;                /* advance 5^(i-1) to 5^i */
    }
    return a_prev1;
}
