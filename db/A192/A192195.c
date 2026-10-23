/*
 * A192195  Monotonic ordering of nonnegative differences 5^i-7^j, for 40>=
 *          i>=0, j>=0.
 * https://oeis.org/A192195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest distinct nonnegative difference 5^i -
 *            7^j with 0 ≤ i ≤ 40, j ≥ 0 (sorted in ascending order)
 * Range:     u8 none, u16 n <= 4, u32 n <= 34, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   mul_small, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_one.h"
#include "../utils/mul_small.h"

/* Comparator for qsort, works for any seq_t that supports < > operators. */
static int seq_t_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    return (*a < *b) ? -1 : (*a > *b);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (cache == NULL) {
        /* Determine the maximal i to use.  On a 64‑bit build we stop
           when 5^i would overflow; otherwise we use i = 40 as the
           problem statement requires. */
        const idx_t MAX_I = (sizeof(seq_t) <= 8) ? 27 : 40;

        /* Rough estimate of total pairs.  720 is enough for i ≤ 40. */
        const size_t EST = 800;
        seq_t *tmp = (seq_t *)malloc(EST * sizeof(seq_t));
        if (!tmp) return 0;  /* fallback on allocation failure */

        size_t len = 0;
        seq_t pow5 = seq_one();   /* 5^0 */

        for (idx_t i = 0; i <= MAX_I; ++i) {
            seq_t pow7 = seq_one();   /* 7^0 */
            /* Enumerate all j with 7^j ≤ 5^i */
            while (pow7 <= pow5) {
                tmp[len++] = pow5 - pow7;
                pow7 = mul_small(pow7, (unsigned)7);
            }
            /* Prepare 5^(i+1) for next iteration */
            pow5 = mul_small(pow5, (unsigned)5);
        }

        /* Sort and unique the values */
        qsort(tmp, len, sizeof(seq_t), seq_t_cmp);
        cache_len = 0;
        cache = (seq_t *)malloc(len * sizeof(seq_t));
        if (!cache) { free(tmp); return 0; }

        seq_t last;
        int has_last = 0;
        for (size_t i = 0; i < len; ++i) {
            if (!has_last || tmp[i] != last) {
                cache[cache_len++] = tmp[i];
                last = tmp[i];
                has_last = 1;
            }
        }
        free(tmp);
    }

    if (n == 0) return 0;   /* offset is 1, but guard against bad calls */
    if (n <= cache_len) return cache[n - 1];
    /* If n exceeds the sequence length, return the largest known value. */
    return cache[cache_len - 1];
}
