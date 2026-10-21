/*
 * A075870  Numbers k such that 2*k^2 - 4 is a square.
 * https://oeis.org/A075870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 2, a(2) = 10, a(n) = 6*a(n-1) - a(n-2) for n >= 3
 *            (recurrence derived from OEIS)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices less than the offset – behaviour is irrelevant */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)10;

    seq_t a_prev2 = (seq_t)2;   /* a(1) */
    seq_t a_prev1 = (seq_t)10;  /* a(2) */
    seq_t a_curr;

    for (idx_t i = 3; i <= n; ++i) {
        a_curr = 6 * a_prev1 - a_prev2;   /* recurrence */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
