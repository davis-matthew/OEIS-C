/*
 * A022137  Fibonacci sequence beginning 5, 12.
 * https://oeis.org/A022137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 5, a(1) = 12
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)5;
    if (n == 1U) return (seq_t)12;

    seq_t prev2 = (seq_t)5;   /* a(0) */
    seq_t prev1 = (seq_t)12;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2U; i <= n; ++i) {
        cur   = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
