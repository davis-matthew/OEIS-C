/*
 * A008484  Number of partitions of n into parts >= 4.
 * https://oeis.org/A008484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) - p(n-1) - p(n-2) + p(n-4) + p(n-5) - p(n-6) where
 *            p(k) is the partition numbers (p(0)=1, p(k)=0 for k<0). The
 *            function builds p(0..n) on demand and uses the recurrence with
 *            generalized pentagonal numbers for O(n·sqrt(n)) time. Suitable
 *            for n up to at least 10 000 within the time limits
 * Range:     u8 n <= 33, u16 n <= 73, u32 n <= 188, u64 n <= 543,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, size_t */

seq_t a(idx_t n) {
    /* Static cache for partition numbers */
    static seq_t *p = NULL;
    static idx_t psize = 0;               /* number of elements stored */

    /* Grow the cache to hold p[0..n] */
    if (n + 1 > psize) {
        size_t newsize = (size_t)(n + 1);
        seq_t *newp = (seq_t *)realloc(p, newsize * sizeof(seq_t));
        if (!newp) abort();              /* fatal: out of memory */
        p = newp;
        /* compute new partition values */
        for (idx_t i = psize; i <= n; ++i) {
            if (i == 0) {
                p[0] = 1;                 /* p(0)=1 */
                continue;
            }
            seq_t sum = 0;
            for (idx_t k = 1;; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                if (pent1 > i) break;
                int sign = (k & 1) ? 1 : -1;
                if (sign > 0) sum += p[i - pent1];
                else          sum -= p[i - pent1];

                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > i) continue;
                if (sign > 0) sum += p[i - pent2];
                else          sum -= p[i - pent2];
            }
            p[i] = sum;
        }
        psize = newsize;
    }

    /* Compute a(n) using the recurrence */
    seq_t res = p[n];
    if (n >= 1) res -= p[n - 1];
    if (n >= 2) res -= p[n - 2];
    if (n >= 4) res += p[n - 4];
    if (n >= 5) res += p[n - 5];
    if (n >= 6) res -= p[n - 6];
    return res;
}
