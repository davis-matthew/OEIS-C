/*
 * A159736  Number of permutations of 6 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 1 local maximum.
 * https://oeis.org/A159736
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n*7^(n-2) (or equivalently a(n) = 14*a(n-1) -
 *            49*a(n-2))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: the first defined term is a(2).  For any n < 2
       the sequence is undefined; return 0 for safety. */
    if (n < 2)
        return 0;

    /* Base values from the OEIS entry */
    if (n == 2)
        return (seq_t)12;
    if (n == 3)
        return (seq_t)126;

    /* Use the linear recurrence a(n) = 14*a(n-1) - 49*a(n-2) */
    seq_t a_prev2 = (seq_t)12;   /* a(2) */
    seq_t a_prev1 = (seq_t)126;  /* a(3) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i)
    {
        cur = (seq_t)14 * a_prev1 - (seq_t)49 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }

    return cur;
}
