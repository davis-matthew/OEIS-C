/*
 * A022384  Fibonacci sequence beginning 4, 18.
 * https://oeis.org/A022384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 4, a(1) = 18
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial two terms directly. */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)18;

    /* Iterate the recurrence a(n) = a(n-1) + a(n-2). */
    seq_t prev2 = (seq_t)4;   /* a(0) */
    seq_t prev1 = (seq_t)18;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr  = prev1 + prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
