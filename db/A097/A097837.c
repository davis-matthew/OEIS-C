/*
 * A097837  Chebyshev polynomials S(n,51) + S(n-1,51) with Diophantine
 *          property.
 * https://oeis.org/A097837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 51*a(n-1) - a(n-2), a(0)=1, a(1)=52
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)52;

    /* Use two variables to keep the last two terms of the recurrence. */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)52;  /* a(1) */
    seq_t a_cur;

    /* Iteratively compute from n=2 up to the requested n. */
    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 51*a(i-1) - a(i-2) */
        a_cur = (seq_t)51 * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_prev1;
}
