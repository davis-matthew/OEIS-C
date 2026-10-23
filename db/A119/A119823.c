/*
 * A119823  a(n) = a(n-1) + number of distinct digits of a(n-1) in decimal
 *          representation, a(0)=0.
 * https://oeis.org/A119823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n-1) + number of distinct decimal digits of a(n-1) with
 *            a(0) = 0. The function memoizes the sequence in a static cache
 *            so that calls can be made in any order
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Count the number of distinct decimal digits in a value.
   Works for any unsigned integer type seq_t, including arbitrary‑precision
   types for which the operators / and % are defined. */
static seq_t count_distinct_digits(seq_t num) {
    unsigned char seen[10] = {0};
    seq_t distinct = 0;

    /* Handle zero explicitly – its representation is “0”
       which has one distinct digit. */
    if (num == (seq_t)0) {
        seen[0] = 1;
        distinct = (seq_t)1;
        return distinct;
    }

    while (num != (seq_t)0) {
        seq_t rem = num % (seq_t)10;
        unsigned char d = (unsigned char)rem;
        if (!seen[d]) {
            seen[d] = 1;
            distinct += (seq_t)1;
        }
        num = num / (seq_t)10;
    }
    return distinct;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* holds a[0] .. a[cached_up_to] */
    static idx_t cached_up_to = 0;       /* index of last cached value */
    static size_t cache_size = 0;        /* allocated capacity in elements */

    /* Initialize cache on first call. */
    if (cache == NULL) {
        cache_size = 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) {
            /* Out‑of‑memory: return 0 as a safe fallback. */
            return (seq_t)0;
        }
        cache[0] = (seq_t)0;
        cached_up_to = 0;
    }

    /* If the requested index is already cached, return it. */
    if (n <= cached_up_to)
        return cache[n];

    /* Grow the cache if necessary. */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size;
        while ((size_t)n >= new_size)
            new_size <<= 1;                 /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) {
            /* Out‑of‑memory: return 0 as a safe fallback. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute values from (cached_up_to + 1) up to n. */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t prev     = cache[i - 1];
        seq_t distinct = count_distinct_digits(prev);
        cache[i] = prev + distinct;
    }

    cached_up_to = n;
    return cache[n];
}
