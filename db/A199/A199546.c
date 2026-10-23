/*
 * A199546  Sorted number of edges of distinct solutions in the mix of regular
 *          convex polyhedra.
 * https://oeis.org/A199546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = sorted list of 2^(k-1) * product of the edge numbers of
 *            each of the k distinct regular convex polyhedra, for all subsets
 *            of size k ≥ 2, where the polyhedra have edge counts {6, 12, 12,
 *            30, 30}
 * Range:     u8 none, u16 n <= 15, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* comparison routine for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* there are 5 polyhedra; all non‑empty subsets of size ≥2 give
       26 distinct mixes, so the sequence has 26 terms. */
    static seq_t cache[26];
    static int ready = 0;
    if (!ready) {
        const seq_t edges[5] = {6, 12, 12, 30, 30};
        size_t count = 0;
        for (uint32_t mask = 0; mask < (1U << 5); ++mask) {
            int k = __builtin_popcount(mask);
            if (k < 2) continue;
            seq_t prod = 1;
            for (int i = 0; i < 5; ++i)
                if (mask & (1U << i))
                    prod *= edges[i];
            seq_t factor = 1;
            for (int i = 0; i < k - 1; ++i)
                factor *= 2;
            cache[count++] = prod * factor;
        }
        /* sort the list */
        qsort(cache, count, sizeof(seq_t), cmp_seq_t);
        ready = 1;
    }
    if (n < 1 || n > 26) return 0;
    return cache[n - 1];
}
