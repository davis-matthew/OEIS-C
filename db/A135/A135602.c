/*
 * A135602  Right-angled numbers with an internal digit as the vertex.
 * https://oeis.org/A135602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 206, u64 n <= 525, u128 n <= 525,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generate all numbers whose decimal digits form a
   right‑angled angle with exactly one internal vertex (change of
   direction).  The digits differ by ±1, the first digit is non‑zero,
   each intermediate digit occurs once, and the sequence has at
   most 19 digits (the largest term is 9876543210123456789). */

#include <stdlib.h>
#include <stdint.h>

static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;

    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Static cache of all sequence values, built on first call. */
    static seq_t *cache   = NULL;
    static size_t cache_size = 0;

    if (cache == NULL) {
        /* Estimate a small initial capacity; the actual count is
           well below a few thousand. */
        size_t cap = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;
        cache_size = 0;

        for (int d1 = 1; d1 <= 9; ++d1) {
            /* Try both initial directions: +1 and -1. */
            for (int inc_sign = -1; inc_sign <= 1; inc_sign += 2) {
                int inc = inc_sign;
                /* The second digit must be in 0..9. */
                if (d1 + inc < 0 || d1 + inc > 9) continue;

                /* Maximum steps before the vertex. */
                int steps_before_max = (inc == 1) ? (9 - d1) : d1;
                if (steps_before_max < 1) continue;

                for (int steps_before = 1; steps_before <= steps_before_max; ++steps_before) {
                    int dv = d1 + inc * steps_before;   /* Vertex digit */
                    int etc = -inc;                      /* Direction after the vertex */
                    int steps_after_max = (etc == 1) ? (9 - dv) : dv;
                    if (steps_after_max < 1) continue;

                    for (int steps_after = 1; steps_after <= steps_after_max; ++steps_after) {
                        /* Build the number digit by digit. */
                        seq_t val = d1;
                        for (int i = 1; i <= steps_before; ++i)
                            val = val * 10 + (d1 + inc * i);
                        for (int j = 1; j <= steps_after; ++j)
                            val = val * 10 + (dv + etc * j);

                        /* Ensure capacity. */
                        if (cache_size == cap) {
                            cap *= 2;
                            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                            if (!tmp) { free(cache); return 0; }
                            cache = tmp;
                        }
                        cache[cache_size++] = val;
                    }
                }
            }
        }

        /* Sort numerically. */
        qsort(cache, cache_size, sizeof(seq_t), cmp_seq_t);

        /* Remove duplicates (unlikely but safe). */
        size_t unique_size = 0;
        for (size_t i = 0; i < cache_size; ++i) {
            if (unique_size == 0 || cache[i] != cache[unique_size - 1])
                cache[unique_size++] = cache[i];
        }
        cache_size = unique_size;
    }

    if (n < 1 || (size_t)n > cache_size)
        return 0;   /* Undefined for out‑of‑range indices. */

    return cache[(size_t)n - 1];
}
