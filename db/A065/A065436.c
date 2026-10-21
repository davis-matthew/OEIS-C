/*
 * A065436  Duplicate of A055500.
 * https://oeis.org/A065436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) is the largest prime ≤ a(n‑1)+a(n‑2) with a(1)=a(2)=1
 * Range:     i8 n <= 12, i16 n <= 21, i32 n <= 21, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;         /* 1‑based array, 0 index unused */
static size_t cache_cap = 0;        /* allocated capacity (elements) */

/* Return a(n) for any n ≥ 1, computing values lazily and memoizing them. */
seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined, but avoid UB */
        return (seq_t)0;

    /* Resize cache if needed */
    if ((size_t)n + 1 > cache_cap) {
        size_t old_cap = cache_cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort();            /* allocation failure */
        }
        cache = tmp;
        cache_cap = new_cap;
        /* zero‑initialize new entries */
        for (size_t i = old_cap; i < new_cap; ++i) {
            cache[i] = (seq_t)0;
        }
    }

    /* Ensure initial conditions */
    if (cache[1] == (seq_t)0) cache[1] = (seq_t)1;
    if (n >= 2 && cache[2] == (seq_t)0) cache[2] = (seq_t)1;

    /* Compute missing terms up to n */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        seq_t cand = sum;

        /* Make cand odd if it is > 2 and even */
        if (cand > (seq_t)2 && (cand & (seq_t)1) == (seq_t)0)
            cand -= (seq_t)1;

        while (!is_prime_big(cand) && cand > (seq_t)2) {
            cand -= (seq_t)2;   /* skip even numbers */
        }
        /* At this point either cand == 2 or cand is prime */
        cache[i] = cand;
    }

    return cache[n];
}
