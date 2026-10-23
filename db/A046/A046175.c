/*
 * A046175  Indices of triangular numbers which are also pentagonal.
 * https://oeis.org/A046175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - a(n-2) + 6 Initial values: a(0)=0, a(1)=1
 *            (OEIS A046175 – indices of triangular numbers that are also
 *            pentagonal)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Iteratively compute the recurrence up to n */
    seq_t prev2 = (seq_t)0;          /* a(n-2) */
    seq_t prev1 = (seq_t)1;          /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * ((seq_t)14) - prev2 + ((seq_t)6);
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;   /* prev1 now holds a(n) */
}
