/*
 * A081574  Fourth binomial transform of Fibonacci numbers F(n).
 * https://oeis.org/A081574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 9*a(n-1) - 19*a(n-2), a(0)=0, a(1)=1
 *
 * Computes the nth term of OEIS A081574 using the linear recurrence.
 * The implementation uses only arithmetic on type seq_t, avoiding
 * intermediate overflow by never casting to a narrower type.
 */
seq_t a(idx_t n)
{
    /* Handle the initial two values explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_prev2 = (seq_t)0;   /* a(0) */
    seq_t a_prev1 = (seq_t)1;   /* a(1) */
    seq_t a_curr;               /* a(i) */

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 9*a(i-1) - 19*a(i-2) */
        a_curr = (seq_t)9 * a_prev1 - (seq_t)19 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_prev1;   /* a(n) */
}
