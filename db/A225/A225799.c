/*
 * A225799  a(n) = Sum_{k=0..n} binomial(n,k) * 10^(n-k) * Fibonacci(n+k).
 * https://oeis.org/A225799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 13*a(n-1) + 109*a(n-2), a(0)=0, a(1)=11 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)11;

    seq_t a_prev2 = (seq_t)0;   /* a(0) */
    seq_t a_prev1 = (seq_t)11;  /* a(1) */
    seq_t a_cur   = 0;

    for (idx_t i = 2; i <= n; ++i) {
        a_cur = (seq_t)13 * a_prev1 + (seq_t)109 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_prev1;
}
