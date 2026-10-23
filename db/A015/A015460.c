/*
 * A015460  q-Fibonacci numbers for q=3, scale a(n-2).
 * https://oeis.org/A015460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3^(n-2) * a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t pow3  = (seq_t)1;   /* 3^(n-2) at the start of the loop (i=2) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 + pow3 * prev2;   /* a(i) = a(i-1) + 3^(i-2) * a(i-2) */
        /* shift for next iteration */
        prev2 = prev1;
        prev1 = cur;
        pow3 *= (seq_t)3;             /* next power: 3^(i-1) */
    }
    return cur;
}
