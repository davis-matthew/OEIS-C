/*
 * A015204  Sum of Gaussian binomial coefficients for q=16.
 * https://oeis.org/A015204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2 a(n) = 2*a(n-1) + a(n-2)*((16^(n-1)) - 1) for
 *            n >= 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Current and previous terms of the recurrence. */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)2;   /* a(1) */

    /* 16^k, starting with 16^1 for the first iteration. */
    seq_t pow16 = (seq_t)1;     /* will be multiplied by 16 at loop start
                                   so that pow16 = 16^(k-1) when computing a(k) */

    /* Compute successive terms up to a(n). */
    for (idx_t i = 2; i <= n; ++i) {
        /* Update power of 16: now pow16 = 16^(i-1) */
        pow16 = pow16 * (seq_t)16;

        /* a(i) = 2*a_prev1 + a_prev2*(pow16 - 1) */
        seq_t term = (seq_t)2 * a_prev1 + a_prev2 * (pow16 - (seq_t)1);

        /* Shift the window for the next iteration. */
        a_prev2 = a_prev1;
        a_prev1 = term;
    }

    /* After the loop, a_prev1 holds a(n). */
    return a_prev1;
}
