/*
 * A205137  Least k such that n divides a difference between distinct
 *          pentagonal numbers when the differences are ordered as in A205136.
 * https://oeis.org/A205137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n divides P(m) – P(t) when the
 *            differences P(m)-P(t) (m>t) are listed by increasing larger
 *            index m and, for a fixed m, by increasing smaller index t (t = 1
 *            .. m-1)
 * Range:     u8 n <= 26, u16 n <= 728, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)    /* undefined offset, but guard for safety */
        return 0;

    /* count of differences produced so far */
    seq_t count = 0;

    /* iterate over larger index m, starting from 2 */
    for (idx_t m = 2; ; ++m)
    {
        /* inner loop over smaller index t */
        for (idx_t t = 1; t < m; ++t)
        {
            ++count;  /* new difference in the sequence */

            /* compute difference: (m-t)*(3*(m+t)-1)/2  */
            seq_t dt = (seq_t)(m - t);
            seq_t sum = (seq_t)(m + t);
            seq_t diff = dt * ((seq_t)3 * sum - 1) / 2;

            /* check divisibility */
            if (diff % ((seq_t)n) == 0)
                return count;
        }
    }
}
