/*
 * A089797  Number of rational knots and links with unknotting (unlinking)
 *          number U=1.
 * https://oeis.org/A089797
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(2m) = 1, m = 1 or m = 2 2^(m-1) - 1, m odd and m ≥ 3 2^(m-1) -
 *            2, m even and m ≥ 4 a(2m+1) = 1, m = 1 2, m = 2 5·2^(m-3) – 1, m
 *            ≥ 3 (here m = ⌊n/2⌋ for even n, m = (n-1)/2 for odd n)
 * Range:     u8 n <= 18, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2.  For n < 2 return 0 (unused). */
    if (n < 2)
        return (seq_t)0;

    /* Even n : n = 2m */
    if ((n & 1) == 0) {
        idx_t m = n >> 1;                   /* m = n/2 */
        if (m <= 2)                         /* m = 1 or 2 */
            return (seq_t)1;

        /* Compute 2^(m-1) */
        seq_t pow2 = (seq_t)1;
        idx_t exp = m - 1;
        for (idx_t i = 0; i < exp; ++i)
            pow2 *= (seq_t)2;

        if ((m & 1) != 0)   /* m odd */
            return pow2 - (seq_t)1;
        else                /* m even (m ≥ 4) */
            return pow2 - (seq_t)2;
    }

    /* Odd n : n = 2m+1 */
    else {
        idx_t m = (n - 1) >> 1;                /* m = (n-1)/2 */

        if (m <= 1)                            /* m = 1 */
            return (seq_t)1;
        if (m == 2)                            /* m = 2 */
            return (seq_t)2;

        /* Compute 5·2^(m-3) – 1  for m ≥ 3 */
        seq_t pow2 = (seq_t)1;
        idx_t exp = m - 3;
        for (idx_t i = 0; i < exp; ++i)
            pow2 *= (seq_t)2;

        return pow2 * (seq_t)5 - (seq_t)1;
    }
}
