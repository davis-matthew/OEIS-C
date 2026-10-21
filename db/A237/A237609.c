/*
 * A237609  Positive integers k such that x^2 - 9xy + y^2 + k = 0 has integer
 *          solutions.
 * https://oeis.org/A237609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   cmp_seq_t, min_k_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence a(n) = k such that x^2 - 9xy + y^2 + k = 0 has integer solutions.
 * Generation by enumerating integer parameters y (positive) and d
 * (discriminant square root) with the same parity as y:
 *   k = (77*y^2 - d^2)/4.
 * For each y, d runs over all integers having the required parity up to
 * sqrt(77)*y. The resulting k-values are collected, sorted, and deduplicated.
 * To answer a(n) efficiently, we increase the maximum y processed until the
 * smallest k that could appear from larger y is larger than the current n‑th
 * value. This guarantees that the first n values are computed correctly.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/cmp_seq_t.h"
#include "../utils/min_k_next.h"

static seq_t *cache = NULL;      /* sorted unique k-values */
static size_t cache_len = 0;     /* number of stored values */

/* build cache of all k-values for y = 1 .. Ymax */
static void build_cache(idx_t Ymax)
{
    size_t cap = 128;
    seq_t *raw = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t cnt = 0;

    for (idx_t y = 1; y <= Ymax; ++y) {
        unsigned __int128 yy = (unsigned __int128)y * y;
        unsigned __int128 limit = (unsigned __int128)77 * yy;

        /* integer square root of limit */
        double sqrt_val = sqrt((double)limit);
        unsigned long long dmax = (unsigned long long)sqrt_val;
        while ((unsigned __int128)(dmax + 1) * (dmax + 1) <= limit) dmax++;
        while ((unsigned __int128)dmax * dmax > limit) dmax--;

        unsigned long long parity = y & 1ULL;
        if ((dmax & 1ULL) != parity) dmax--;

        for (unsigned long long d = parity; d <= dmax; d += 2) {
            unsigned __int128 dsq = (unsigned __int128)d * d;
            unsigned __int128 k128 = (limit - dsq) / 4;
            seq_t k = (seq_t)k128;

            if (cnt == cap) {
                cap <<= 1;
                raw = (seq_t *)realloc(raw, cap * sizeof(seq_t));
            }
            raw[cnt++] = k;
        }
    }

    /* sort the raw list */
    qsort(raw, cnt, sizeof(seq_t), cmp_seq_t);

    /* remove duplicates */
    size_t uniq = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (i == 0 || raw[i] != raw[i - 1]) {
            raw[uniq++] = raw[i];
        }
    }

    /* reallocate cache with the exact size */
    seq_t *tmp = (seq_t *)realloc(raw, uniq * sizeof(seq_t));
    free(cache);
    cache = tmp;
    cache_len = uniq;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n = 0 */

    /* if we already have enough terms, return the n-th value */
    if (cache_len >= n) return cache[n - 1];

    /* grow cache until the first n terms are finalized */
    idx_t Y = 1;
    while (1) {
        build_cache(Y);
        if (cache_len >= n) {
            seq_t nth_val = cache[n - 1];
            seq_t next_min = min_k_next(Y + 1);
            if (next_min > nth_val) break;   /* guaranteed that n-th term is correct */
        }
        Y++;                                 /* handle more y-values */
    }
    return cache[n - 1];
}
