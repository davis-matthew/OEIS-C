/*
 * A034157  Number of partitions of n into 3 distinct parts from [ 1,14 ].
 * https://oeis.org/A034157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 distinct parts from
 *            [1,14]
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The largest possible sum is 12 + 13 + 14 = 39, so for n > 39 the
       answer is zero.  The total number of unordered triples is
       C(14,3) = 364, which is tiny, so a straightforward triple loop is
       efficient enough for all practical n. */
    seq_t count = 0;
    for (idx_t i = 1; i <= 14; ++i)
    {
        for (idx_t j = i + 1; j <= 14; ++j)
        {
            for (idx_t k = j + 1; k <= 14; ++k)
            {
                if (i + j + k == n)
                    ++count;
            }
        }
    }
    return count;
}
