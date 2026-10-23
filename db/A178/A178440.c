/*
 * A178440  Convolution square of A058187, the tetrahedral series with
 *          repeats.
 * https://oeis.org/A178440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 138, u64 n <= 3454,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function 1 / ((1+x)^6·(1‑x)^8).
   a(0) = 1;
   For n ≥ 1:
   a(n) = Σ_{k=1}^{14} d[k] · a(n‑k),
   where d = {2, 5, -12, -9, 30, 5, -40, 5, 30, -9, -12, 5, 2, -1}. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

static const int d[15] = {0, 2, 5, -12, -9, 30, 5, -40, 5, 30, -9, -12, 5, 2, -1};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cached = 0;

    /* Ensure the cache holds values up to index n. */
    if (n >= cached) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* Compute missing terms. */
        for (idx_t i = cached; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;
                continue;
            }

            seq_t pos = 0;  /* Sum of positive contributions */
            seq_t neg = 0;  /* Sum of negative contributions */

            /* Compute the recurrence using known terms. */
            for (idx_t k = 1; k <= 14 && k <= i; ++k) {
                seq_t term = cache[i - k];
                int ck = d[k];
                if (ck > 0) {
                    pos = pos + mul_small(term, (unsigned int)ck);
                } else {
                    neg = neg + mul_small(term, (unsigned int)(-ck));
                }
            }

            /* a(i) = pos - neg; pos is guaranteed ≥ neg. */
            cache[i] = pos - neg;
        }
        cached = new_cap;
    }

    return cache[(size_t)n];
}
