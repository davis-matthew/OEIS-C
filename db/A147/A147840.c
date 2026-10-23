/*
 * A147840  a(n)=10*a(n-1)-8*a(n-2), a(0)=1, a(1)=8 .
 * https://oeis.org/A147840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 8*a(n-2), a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)8;

    /* Iteratively compute the recurrence using two variables only. */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)8;   /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)10 * prev1 - (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
