/*
 * A156575  a(n) = 34*a(n-1)-a(n-2)-4232 for n > 2; a(1)=289, a(2)=4225.
 * https://oeis.org/A156575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1)-a(n-2)-4232 for n>2, with a(1)=289, a(2)=4225
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0) return 0;           /* outside offset, not used */
    if (n == 1) return 289;
    if (n == 2) return 4225;

    /* Iteratively apply the recurrence relation */
    seq_t prev2 = 289;  /* a(1) */
    seq_t prev1 = 4225; /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)34 * prev1 - prev2 - 4232;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
