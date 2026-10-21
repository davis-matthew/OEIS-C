/*
 * A093446  Largest member of the n-th row of the triangular triangle
 *          (A093445).
 * https://oeis.org/A093446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum among the sums of blocks of consecutive integers
 *            obtained by partitioning the sequence 1,2,3,... into blocks of
 *            lengths n, n-1, ..., 1. For each n ≥ 1 this gives the required
 *            a(n)
 * Range:     u8 n <= 10, u16 n <= 69, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* offset 1: a(1) = 1, lower values unused */

    seq_t maxSum = 0;      /* largest sum seen so far          */
    seq_t start  = 1;      /* starting value of the current block */

    for (idx_t len = n; len >= 1; --len) {
        seq_t sum = 0;
        seq_t cur = start;

        /* compute the sum of the current block of length len */
        for (idx_t i = 0; i < len; ++i) {
            sum += cur;
            ++cur;
        }

        if (sum > maxSum)
            maxSum = sum;

        /* advance to the next block */
        start += len;
    }

    return maxSum;
}
