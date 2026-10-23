/*
 * A028172  Expansion of 1/((1-5*x)*(1-6*x)*(1-8*x)*(1-11*x)).
 * https://oeis.org/A028172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(n+1) - 5^(n+1) + 19*a(n-1) - 88*a(n-2); (a(0)=1,
 *            a(1)=30)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)30;

    seq_t a_prev2 = (seq_t)1;        /* a(0) */
    seq_t a_prev1 = (seq_t)30;       /* a(1) */
    seq_t a_curr = a_prev2;

    seq_t pow5 = (seq_t)25;          /* 5^2  */
    seq_t pow6 = (seq_t)36;          /* 6^2  */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = pow6 * (seq_t)6 - pow5 * (seq_t)5;          /* 6^(i+1) - 5^(i+1) */
        a_curr = term + (seq_t)19 * a_prev1 - (seq_t)88 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
        pow6 *= (seq_t)6;
        pow5 *= (seq_t)5;
    }
    return a_curr;
}
