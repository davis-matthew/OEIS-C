/*
 * A211026  Number of segments needed to draw on the infinite square grid a
 *          diagram of regions and partitions of n.
 * https://oeis.org/A211026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000041(n) + 2 (p(n) = partition number of n)
 * Range:     u8 n <= 13, u16 n <= 39, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes a(n) for OEIS A211026. */
seq_t a(idx_t n)
{
    /* cache of partition numbers p[0..max_n] */
    static seq_t *p = NULL;
    static idx_t max_n = 0;      /* largest n already computed */
    static size_t cap = 0;       /* allocated capacity */

    if (n == 0)                /* undefined for n=0, return 0 */
        return 0;

    /* enlarge cache if needed */
    if (n > max_n) {
        size_t new_cap = n + 1;
        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(p, new_cap * sizeof(seq_t));
            if (!tmp) return 0;      /* allocation failure: signal error */
            p = tmp;
            /* zero only the new part of the array */
            for (idx_t i = cap; i < new_cap; ++i)
                p[i] = 0;
            cap = new_cap;
        }

        /* initialise p[0] once */
        if (max_n == 0)
            p[0] = 1;

        /* compute partition numbers for (max_n+1)..n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t pos = 0, neg = 0;
            for (idx_t k = 1; ; ++k) {
                idx_t g1 = k * (3 * k - 1) / 2;
                if (g1 > i) break;
                if (k & 1) pos += p[i - g1];
                else       neg += p[i - g1];

                idx_t g2 = k * (3 * k + 1) / 2;
                if (g2 > i) continue;
                if (k & 1) pos += p[i - g2];
                else       neg += p[i - g2];
            }
            p[i] = pos - neg;
        }
        max_n = n;
    }

    /* a(n) = 2 * p(n) + 2 */
    return (p[n] << 1) + 2;
}
