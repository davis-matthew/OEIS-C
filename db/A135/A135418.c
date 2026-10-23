/*
 * A135418  Number of ways of splitting the numbers from 1 to 16 into two
 *          groups so that the numbers in each group have the same sum of n-th
 *          powers.
 * https://oeis.org/A135418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered splits of {1,…,16} into two groups
 *            having equal sum of the n‑th powers of the elements
 * Range:     u8 none, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* below offset, undefined */
    if (n > 4) return 0;                      /* no solutions for n ≥ 5 */

    static int ready = 0;
    static seq_t tbl[5];                      /* indices 1..4 */

    if (!ready) {
        /* precompute for n = 1..4 */
        for (idx_t k = 1; k <= 4; ++k) {
            seq_t power[16];
            seq_t total = 0;
            /* compute i^k for i = 1..16 */
            for (idx_t i = 1; i <= 16; ++i) {
                seq_t p = 1;
                for (idx_t e = 0; e < k; ++e) p *= i;
                power[i - 1] = p;
                total += p;
            }
            if ((total & 1) == 0) {
                seq_t half = total >> 1; /* total/2 */
                seq_t cnt = 0;
                const unsigned long long all_masks = 1ULL << 16; /* 65536 */
                for (unsigned long long m = 0; m < all_masks; ++m) {
                    seq_t s = 0;
                    for (int j = 0; j < 16; ++j)
                        if (m & (1ULL << j)) s += power[j];
                    if (s == half) ++cnt;
                }
                tbl[k] = cnt >> 1;            /* divide by 2 for unordered splits */
            } else {
                tbl[k] = 0;
            }
        }
        ready = 1;
    }
    return tbl[n];
}
