/*
 * A077246  Bisection (even part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - a(n-2) with a(0) = 2, a(1) = 13
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial two terms directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)13;

    /* Iterate using the linear recurrence */
    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)13;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = prev1 * ((seq_t)8) - prev2; /* 8*a(i-1) - a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
