/*
 * A088100  Group the natural numbers with at least two members in each group
 *          such that the n-th group sum is a multiple of n. (1,2), (3,4,5),
 *          (6,7,8), (9,10,11,12,13,14,15), (16,17,18,19),
 *          (20,21,22,23,24,25,26,27,28), (29,30,31,32,33,34), ... Sequence
 *          contains the number of members in the n-th group.
 * https://oeis.org/A088100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = size of the n‑th group in the greedy partition of the
 *            natural numbers in which each group has at least two consecutive
 *            terms and the sum of a group is a multiple of its index. The
 *            function computes this by iterating sequentially from group 1 up
 *            to n; it needs no extra storage or caching
 * Range:     u8 n <= 8, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n = 0 */

    seq_t start = (seq_t)1; /* starting number of the current group */

    for (idx_t grp = 1; grp <= n; ++grp)
    {
        /* Find the smallest m ≥ 2 so that
           sum = m*(2*start + m - 1)/2 is divisible by grp. */
        seq_t m = (seq_t)2;
        while (1)
        {
            seq_t sum = m * ( ((seq_t)2 * start) + m - (seq_t)1 ) / (seq_t)2;
            if ((sum % (seq_t)grp) == (seq_t)0) break;
            ++m;
        }

        if (grp == n) return m; /* answer found */

        /* Advance to the next group */
        start += m;
    }

    /* should never reach here */
    return (seq_t)0;
}
