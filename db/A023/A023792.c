/*
 * A023792  Katadromes: digits in base 11 are in strict descending order.
 * https://oeis.org/A023792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = n-th smallest integer whose base‑11 digits are strictly
 *            decreasing
 * Range:     u8 n <= 1, u16 n <= 438, u32 n <= 1998, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Internal cache to store the 2047 values. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Comparison function for qsort. */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Build the cache on first call. */
static void build_cache(void)
{
    const unsigned digits = 11;                 /* digits 0 … 10 */
    const size_t total   = (1u << digits) - 1;   /* 2047 subsets */

    cache = (seq_t *)malloc(total * sizeof(seq_t));
    if (!cache) return; /* Should never happen for such a small allocation. */

    for (size_t mask = 1; mask <= total; ++mask) {
        seq_t val = 0;
        for (int d = digits - 1; d >= 0; --d) {
            if (mask & (1u << d)) {
                val = val * (seq_t)11 + (seq_t)d;
            }
        }
        cache[mask - 1] = val; /* zero‑based index */
    }

    qsort(cache, total, sizeof(seq_t), cmp_seq);
    cache_size = total;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (!cache) build_cache();

    if (n < 1 || (size_t)n > cache_size) {
        return (seq_t)0;
    }

    return cache[n - 1];
}
