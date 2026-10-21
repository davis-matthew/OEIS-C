/*
 * A228087  a(n) = largest k which satisfies n = k + bitcount(k), or 0 if no
 *          such k exists. Here bitcount(k) (A000120) gives the number of 1's
 *          in binary representation of nonnegative integer k.
 * https://oeis.org/A228087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest k such that n = k + popcount(k)
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline unsigned int popcount_seq(seq_t v)
{
    unsigned int c = 0;
    /* count bits for all possible seq_t sizes */
    while (v) {
        c += (unsigned int)(v & (seq_t)1);
        v >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Search k from n down to 0, returning the first that satisfies
     * n = k + popcount(k).  This is the largest such k. */
    for (idx_t k_idx = n; ; --k_idx) {
        seq_t k = (seq_t)k_idx;
        unsigned int pc = popcount_seq(k);
        if ((unsigned int)(n - k_idx) == pc)
            return k;
        if (k_idx == 0)          /* reached the smallest candidate */
            break;
    }
    return 0;
}
