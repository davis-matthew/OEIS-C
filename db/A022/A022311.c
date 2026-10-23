/*
 * A022311  a(n) = a(n-1) + a(n-2) + 1 for n>1, a(0)=0, a(1)=1.
 * https://oeis.org/A022311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 6, a(n) = a(n-1) + a(n-2) + 1 (n > 1)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)6;

    /* Iterate from the start up to n, keeping only the last two values. */
    seq_t prev2 = (seq_t)0;      /* a(0) */
    seq_t prev1 = (seq_t)6;      /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-2) + 1 */
        curr = prev1 + prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
