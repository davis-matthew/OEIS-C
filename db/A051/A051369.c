/*
 * A051369  a(n+1) = a(n) + sum of digits of a(n)^2, with a(1) = 3.
 * https://oeis.org/A051369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(1) = 3; for n ≥ 1: a(n+1) = a(n) + sum_of_digits(a(n)²)
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 3119, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                    /* behaviour below the offset is irrelevant */

    static seq_t *cache = NULL;       /* 1‑based cache; cache[0] unused */
    static size_t capacity = 0;      /* allocated capacity (number of elements) */
    static size_t size = 0;          /* highest index that has been computed */

    /* Ensure the cache is large enough for indices 1 … n */
    if (capacity < (size_t)n + 1) {
        size_t new_cap = capacity ? capacity * 2 : 1;
        while (new_cap < (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the first term if it has not been done yet */
    if (size < 1) {
        cache[1] = (seq_t)3;
        size = 1;
    }

    /* Compute any missing terms up to the requested index n */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t sq   = prev * prev;
        seq_t add  = digit_sum(sq);
        cache[i] = prev + add;
    }

    size = n;
    return cache[n];
}
