/*
 * A020691  Numbers of form 7 x^2 + 9 y^2.
 * https://oeis.org/A020691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   A020691 = numbers of the form 7*x^2 + 9*y^2, sorted ascending
 *            (unique)
 * Range:     u8 n <= 20, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   cmp_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cmp_seq_t.h"

/* external integer types (supplied by the build) */
extern seq_t a(idx_t n);

/* cached sequence values:   cache[0] == a(1) */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static idx_t  cur_limit = 0;  /* largest value used to generate cache */

/* compare function for qsort, using the provided cmp_seq_t */
static int seq_cmp_wrapper(const void *p1, const void *p2)
{
    return cmp_seq_t(p1, p2);
}

/* Ensure the cache contains at least `need` terms.  Generate
   all numbers <= `limit` of the form 7*x^2 + 9*y^2, sort, dedupe. */
static void rebuild_cache(idx_t need)
{
    if (cache_len >= need) return;

    /* Rough initial limit: enough that 20 * need covers most cases. */
    idx_t limit = (cur_limit == 0) ? (need * 20) : cur_limit * 2;
    if (limit == 0) limit = 1;   /* avoid 0 limit */

    while (1) {
        /* Temporary buffer to collect raw values */
        size_t buf_cap = 1024;
        seq_t *buf = (seq_t *)malloc(buf_cap * sizeof(seq_t));
        if (!buf) exit(1);
        size_t buf_len = 0;

        /* Generate values <= limit, using 64‑bit arithmetic for safety. */
        for (idx_t x = 0;; ++x) {
            uint64_t x2 = (uint64_t)x * (uint64_t)x;
            uint64_t part = 7ULL * x2;
            if (part > (uint64_t)limit) break;

            for (idx_t y = 0;; ++y) {
                uint64_t y2 = (uint64_t)y * (uint64_t)y;
                uint64_t val = part + 9ULL * y2;
                if (val > (uint64_t)limit) break;

                if (buf_len == buf_cap) {
                    buf_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(buf, buf_cap * sizeof(seq_t));
                    if (!tmp) { free(buf); exit(1); }
                    buf = tmp;
                }
                buf[buf_len++] = (seq_t)val;
            }
        }

        /* Sort the values. */
        qsort(buf, buf_len, sizeof(seq_t), seq_cmp_wrapper);

        /* Remove duplicates in place. */
        size_t uniq_len = 0;
        for (size_t i = 0; i < buf_len; ++i) {
            if (i == 0 || buf[i] != buf[i-1])
                buf[uniq_len++] = buf[i];
        }

        /* If enough terms collected, store into cache and return. */
        if (uniq_len >= (size_t)need) {
            seq_t *new_cache = (seq_t *)malloc(uniq_len * sizeof(seq_t));
            if (!new_cache) { free(buf); exit(1); }
            for (size_t i = 0; i < uniq_len; ++i) new_cache[i] = buf[i];

            free(buf);
            free(cache);
            cache     = new_cache;
            cache_len = uniq_len;
            cur_limit = limit;
            return;
        }

        /* Not enough – double limit and retry. */
        free(buf);
        limit <<= 1;
    }
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* behavior below offset is irrelevant */

    rebuild_cache(n);
    return cache[n - 1];   /* cache[0] corresponds to a(1) */
}
