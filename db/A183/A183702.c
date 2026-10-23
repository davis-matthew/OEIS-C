/*
 * A183702  Number of (n+1) X 2 0..3 arrays with every 2 X 2 subblock
 *          nonsingular.
 * https://oeis.org/A183702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) + 24*a(n-2) with a(1) = 192, a(2) = 2472. This
 *            implements the recurrence directly, avoiding any dynamic
 *            allocation so that the code compiles both as C11 and as C++17,
 *            even when seq_t is an arbitrary‑precision integer class
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly. */
    if (n == 1) return 192;
    if (n == 2) return 2472;

    /* Compute iteratively up to n. */
    seq_t a_prev2 = 192;      /* a(1)   */
    seq_t a_prev1 = 2472;     /* a(2)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = 11 * a_prev1 + 24 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }

    return a_prev1;
}
