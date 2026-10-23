/*
 * A008632  Molien series for A_9.
 * https://oeis.org/A008632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) + p(n-36), where p(k) = number of partitions of k
 *            into parts ≤ 9 (p(k)=0 for k<0)
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 265, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *p = NULL;
    static idx_t cap = 0;

    /* we need values up to max(n, n-36) but max(n-36)<n for n≥36,
       otherwise the bound is just n. */
    idx_t maxN = n;
    if (cap <= maxN) {
        seq_t *new_arr = (seq_t*)malloc((size_t)(maxN + 1) * sizeof(seq_t));
        if (!new_arr) return 0; /* out of memory: return 0 */
        if (p) free(p);
        p = new_arr;
        cap = maxN + 1;

        /* initialise to zero */
        for (idx_t i = 0; i <= maxN; ++i) p[i] = 0;
        p[0] = 1;

        /* dynamic programming for partitions with parts 1..9 */
        for (int part = 1; part <= 9; ++part) {
            for (idx_t j = part; j <= maxN; ++j) {
                p[j] = p[j] + p[j - part];
            }
        }
    }

    seq_t val = p[n];
    if (n >= 36) val = val + p[n - 36];
    return val;
}
