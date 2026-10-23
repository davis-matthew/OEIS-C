/*
 * A023777  Metadromes: digits in base 8 are in strict ascending order.
 * https://oeis.org/A023777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = nth smallest nonnegative integer whose octal digits are
 *            strictly increasing
 * Range:     u8 n <= 27, u16 n <= 120, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper comparator for qsort */
static int seq_cmp(const void *p1, const void *p2) {
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

seq_t a(idx_t n) {
    /* Cache for the generated sequence */
    static seq_t cache[128];
    static size_t cache_len = 0;

    if (cache_len == 0) {
        /* Generate all numbers with strictly increasing base‑8 digits (digits 1–7) */
        size_t idx = 0;
        cache[idx++] = (seq_t)0;            /* n = 1 -> 0 */
        /* Loop over all non‑empty subsets of {1,…,7} */
        for (uint32_t mask = 1; mask < (1u << 7); ++mask) {
            unsigned long long val = 0;     /* use a wider type for intermediate */
            for (int d = 0; d < 7; ++d) {
                if (mask & (1u << d)) {
                    val = val * 8 + (unsigned long long)(d + 1);
                }
            }
            cache[idx++] = (seq_t)val;
        }
        cache_len = idx;                   /* should be 128 */

        /* Sort the array in ascending order of decimal value */
        qsort(cache, cache_len, sizeof(seq_t), seq_cmp);
    }

    /* Handle out-of-range indices (behaviour below offset is irrelevant) */
    if (n == 0 || n > cache_len) {
        return (seq_t)0;
    }
    return cache[n - 1];
}
