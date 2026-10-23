/*
 * A020682  Numbers of form 4 x^2 + 5 y^2.
 * https://oeis.org/A020682
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th number (sorted ascending, duplicates removed) of
 *            the form 4*x^2 + 5*y^2
 * Range:     u8 n <= 1, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/cmp_seq_t.h"

/* Integer square root for uint64_t, used to bound x and y */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t x = n, y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* Cache of generated, sorted unique values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static uint64_t LIMIT = 2000;  /* initial value; will be doubled until enough terms are found */

/* Rebuild the cache to contain at least `need` terms */
static void build_cache(idx_t need)
{
    /* Free old cache if present */
    if (cache) {
        free(cache);
        cache = NULL;
        cache_len = 0;
    }

    /* Build up to current LIMIT; if insufficient, keep doubling until enough */
    while (1) {
        uint64_t max_x = isqrt_u64(LIMIT / 4);
        uint64_t max_y = isqrt_u64(LIMIT / 5);

        /* Estimate total raw values; allocate nevertheless */
        size_t total = (size_t)(max_x + 1) * (size_t)(max_y + 1);
        seq_t *raw = (seq_t *)malloc(total * sizeof(seq_t));
        if (!raw) exit(1);  /* out of memory */

        size_t cnt = 0;
        for (uint64_t x = 0; x <= max_x; ++x) {
            uint64_t t4 = 4 * x * x;
            if (t4 > LIMIT) continue;  /* should not happen, but guard */

            for (uint64_t y = 0; y <= max_y; ++y) {
                uint64_t t5 = 5 * y * y;
                if (t5 > LIMIT) continue;
                uint64_t val64 = t4 + t5;
                if (val64 > LIMIT) continue;
                raw[cnt++] = (seq_t)val64;
            }
        }

        /* Sort and deduplicate */
        qsort(raw, cnt, sizeof(seq_t), cmp_seq_t);

        size_t uniq = 0;
        for (size_t i = 0; i < cnt; ++i) {
            if (i == 0 || cmp_seq_t(&raw[i], &raw[i - 1]) != 0)
                raw[uniq++] = raw[i];
        }

        cache_len = uniq;
        cache = raw;  /* raw now contains the unique sorted list */

        if (cache_len >= (size_t)need) break;

        /* Not enough terms: enlarge LIMIT and retry */
        LIMIT *= 2;
        free(cache);
        cache = NULL;
        cache_len = 0;
    }
}

/* External function required by the OEIS framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined for n < 1, but keep safe */
    if (cache_len < (size_t)n)
        build_cache(n);
    return cache[n - 1];
}
