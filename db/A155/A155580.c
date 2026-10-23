/*
 * A155580  a(n)=If[IntegerQ[((3*n - 2)/( n + 1))*a(n - 1)], ((3*n - 2)/(n +
 *          1))* a(n - 1), If[IntegerQ[((4*n - 2)/(n + 1))*a( n - 1)], ((4*n -
 *          2)/(n + 1))*a(n - 1), n*a(n - 1)]].
 * https://oeis.org/A155580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(0) = 1; a(n) = If[IntegerQ[((3*n - 2)/(n + 1))*a(n - 1)],
 *            ((3*n - 2)/(n + 1))*a(n - 1), If[IntegerQ[((4*n - 2)/(n +
 *            1))*a(n - 1)], ((4*n - 2)/(n + 1))*a(n - 1), n*a(n - 1)]]; (OEIS
 *            A155580)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cache for memoised values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_size > n)
        return;

    /* Allocate or grow the cache */
    if (cache == NULL) {
        /* No cache yet: allocate a new array of size n+1 */
        cache = (seq_t *)realloc(NULL, (n + 1) * sizeof(seq_t));
        if (!cache)
            abort();
        cache_size = 0;
    } else if (n + 1 > cache_size) {
        /* Grow the existing cache */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (n + 1) * sizeof(seq_t));
        if (!new_cache)
            abort();
        cache = new_cache;
    }

    /* Initialize newly added entries */
    if (cache_size == 0) {
        /* a(0) = 1 */
        cache[0] = 1;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) /* already set */ ;
        else {
            seq_t prev = cache[i - 1];
            seq_t denom = (seq_t)(i + 1);

            /* Try ((3*i - 2) / (i + 1)) * prev */
            seq_t num1 = (seq_t)(3 * i - 2) * prev;
            if (num1 % denom == 0) {
                cache[i] = num1 / denom;
                continue;
            }

            /* Try ((4*i - 2) / (i + 1)) * prev */
            seq_t num2 = (seq_t)(4 * i - 2) * prev;
            if (num2 % denom == 0) {
                cache[i] = num2 / denom;
                continue;
            }

            /* Default: n * prev */
            cache[i] = prev * (seq_t)i;
        }
    }

    cache_size = n + 1;
}

/* Return the nth term of the sequence (0‑based). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
