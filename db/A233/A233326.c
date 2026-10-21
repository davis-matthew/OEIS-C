/*
 * A233326  a(n) = (7^(n+1) - 4) / 3.
 * https://oeis.org/A233326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^(n+1) - 4) / 3. Recurrence: a(0) = 1, a(n) = 7*a(n-1)
 *            + 8 for n > 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base case directly. */
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    /* Compute iteratively: a(n) = 7*a(n-1) + 8. */
    seq_t cur = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        cur = cur * (seq_t)7 + (seq_t)8;
    }
    return cur;
}
