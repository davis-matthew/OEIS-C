/*
 * A177240  Number of K-toothpicks after n stages of 3-D K-toothpick structure
 *          defined in Comments.
 * https://oeis.org/A177240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 0; a(2n) = 5 * a(n); a(2n+1) = 5 * a(n) + 4^{popcount(n)}
 *            where popcount(n) is the number of 1‑bits in n
 * Range:     u8 n <= 13, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if ((n & 1) == 0) {                 /* n is even */
        return (seq_t)5 * a(n >> 1);
    } else {                            /* n is odd, n = 2k + 1 */
        idx_t k = (n - 1) >> 1;         /* k = (n-1)/2 */
        seq_t val = a(k);
        /* compute 4^popcount(k) */
        unsigned cnt = 0;
        idx_t temp = k;
        while (temp) {
            temp &= temp - 1;
            ++cnt;
        }
        seq_t inc = 1;
        for (unsigned i = 0; i < cnt; ++i) inc *= (seq_t)4;
        return (seq_t)5 * val + inc;
    }
}
