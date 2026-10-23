/*
 * A194805  Number of parts that are visible in one of the three views of the
 *          modular table of partitions version "tree" with n sections.
 * https://oeis.org/A194805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) + A000041(n-1) - 1 (for n ≥ 1); a(0) = 0
 * Range:     u8 n <= 14, u16 n <= 39, u32 n <= 121, u64 n <= 405,
 *            u128 n <= 1437, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here A000041 is the partition number p(n).  The implementation uses
   the pentagonal number theorem to compute p(n) on demand and caches
   all computed values.  Only integer arithmetic is used. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *p = NULL;            /* cached partition numbers */
    static size_t cap = 0;             /* allocated size of p */
    static idx_t last = 0;             /* largest index computed so far */

    /* initialise cache on first call */
    if (!p) {
        cap = 1;
        p = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!p) return (seq_t)0;
        p[0] = (seq_t)1;              /* p(0) = 1 */
        last = 0;
    }

    /* extend cache up to n if necessary */
    if (n > last) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(p, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            p = tmp;
            cap = new_cap;
        }

        /* compute partition numbers for indices last+1 … n */
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t val = 0;          /* p(i) being built */
            for (idx_t k = 1; ; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                if (pent1 > i) break;
                seq_t add = p[i - pent1];
                if (k % 2 == 1) val += add;
                else val -= add;

                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > i) continue;
                add = p[i - pent2];
                if (k % 2 == 1) val += add;
                else val -= add;
            }
            p[i] = val;            /* store computed partition number */
        }
        last = n;
    }

    /* a(0) = 0 */
    if (n == 0) return (seq_t)0;

    /* a(n) = p(n) + p(n-1) - 1 for n ≥ 1 */
    return p[n] + p[n - 1] - (seq_t)1;
}
