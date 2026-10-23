/*
 * A026644  a(n) = a(n-1) + 2*a(n-2) + 2, for n>=3, where a(0)= 1, a(1)= 2,
 *          a(2)= 4.
 * https://oeis.org/A026644
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 2, for n >= 3 with a(0) = 1, a(1) =
 *            2, a(2) = 4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* For n >= 3 we iterate using the recurrence. */
    seq_t prev2 = (seq_t)2;  /* a(1) */
    seq_t prev1 = (seq_t)4;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        /* a(i) = a(i-1) + 2*a(i-2) + 2 */
        cur = prev1 + ((prev2 << 1) + (seq_t)2);
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
