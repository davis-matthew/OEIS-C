/*
 * A126485  Number of base 17 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = number of length‑n base‑17 numbers whose adjacent digits
 *            differ by at most 3. a(0) = 1, a(1) = 17
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset */
    if (n == 0) return (seq_t)1;

    /* dp arrays for current and previous digit counts (size 17) */
    seq_t cur[17], prev[17];

    /* initialisation for length 1: each digit can appear in a 1‑digit number */
    for (idx_t d = 0; d < 17; ++d)
        cur[d] = (seq_t)1;

    if (n == 1)
    {
        seq_t sum = (seq_t)0;
        for (idx_t d = 0; d < 17; ++d)
            sum += cur[d];
        return sum;          /* 17 */
    }

    /* prepare for iteration: copy cur into prev */
    for (idx_t d = 0; d < 17; ++d)
        prev[d] = cur[d];

    /* iterate for lengths 2 .. n */
    for (idx_t len = 2; len <= n; ++len)
    {
        /* reset current counts */
        for (idx_t d = 0; d < 17; ++d)
            cur[d] = (seq_t)0;

        /* transition from prev to cur */
        for (int j = 0; j < 17; ++j)
        {
            seq_t v = prev[j];

            /* digits within difference <= 3 of j */
            int start = j - 3;
            if (start < 0) start = 0;
            int end   = j + 3;
            if (end > 16) end = 16;

            for (int i = start; i <= end; ++i)
                cur[i] += v;
        }

        /* ready for next round: previous becomes current */
        for (idx_t d = 0; d < 17; ++d)
            prev[d] = cur[d];
    }

    /* after loop, cur holds counts for length n */
    seq_t sum = (seq_t)0;
    for (idx_t d = 0; d < 17; ++d)
        sum += cur[d];
    return sum;
}
