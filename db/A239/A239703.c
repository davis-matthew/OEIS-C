/*
 * A239703  Number of bases b > 1 for which the base-b digital sum of n is b.
 * https://oeis.org/A239703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of bases b>1 such that digitSum_b(n) == b
 * Range:     u8 n <= 1787, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;

    /* Bases larger than n cannot work, so we only need to test 2 … n */
    for (idx_t b = 2; b <= n; ++b)
    {
        seq_t sum = 0;
        idx_t t = n;
        seq_t sb = (seq_t)b;

        /* Compute the digit sum of n in base b.
           Early exit if the sum already exceeds b. */
        while (t > 0)
        {
            sum += (seq_t)(t % b);
            if (sum > sb) break;   /* impossible to become equal later   */
            t /= b;
        }

        if (sum == sb) ++cnt;
    }

    return cnt;
}
