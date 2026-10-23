/*
 * A173497  a(n) = a(n-1) + a(n-2) - floor(a(n-2)/2), starting 2,1.
 * https://oeis.org/A173497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - floor(a(n-2)/2), with a(0) = 2, a(1) =
 *            1
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 70, u64 n <= 141,
 *            u128 n <= 283, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)1;

    /* iterate using only the previous two terms */
    seq_t prev2 = (seq_t)2; /* a(0) */
    seq_t prev1 = (seq_t)1; /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t curr = prev1 + prev2 - (prev2 / (seq_t)2);
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
