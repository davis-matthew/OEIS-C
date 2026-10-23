/*
 * A033298  a(n+1) = a(n) + sum of digits of a(n)^2, with a(1) = 1.
 * https://oeis.org/A033298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   a(1) = 1, a(n+1) = a(n) + sum of digits of a(n)^2 (OEIS A033298)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/digit_sum.h"

/* Static cache of already computed terms.  cache[i] = a(i+1). */
static seq_t *cache      = NULL;  /* dynamically allocated array */
static size_t cache_cap  = 0;     /* allocated capacity (number of seq_t slots) */
static size_t cache_size = 0;     /* number of terms currently stored */

/* Ensure the cache can hold at least n terms (n ≥ 1). */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation: 1 element is enough for a(1) */
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* allocation failure – abort (indeterminate behaviour elsewhere) */
            exit(EXIT_FAILURE);
        }
        cache_size = 0;
    }

    /* Grow until capacity ≥ n.  Double the size each time. */
    while ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < cache_cap) {           /* overflow guard – shouldn't happen */
            new_cap = (size_t)n;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* reallocation failure – abort */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute a(n) for any n ≥ 1, in any order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined by definition – return 0 safely. */
        return (seq_t)0;
    }

    ensure_capacity(n);

    /* Generate terms up to n if needed. */
    while (cache_size < (size_t)n) {
        seq_t previous;
        if (cache_size == 0) {
            /* a(1) is the seed value. */
            previous = (seq_t)1;
            cache[0] = previous;
            cache_size = 1;
            continue;  /* after storing a(1), recompute for next indices */
        } else {
            previous = cache[cache_size - 1];
        }

        seq_t sq  = seq_mul(previous, previous);
        seq_t sum = digit_sum(sq);
        seq_t cur = previous + sum;
        cache[cache_size++] = cur;
    }

    return cache[n - 1];
}
