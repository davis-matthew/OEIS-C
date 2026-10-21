/*
 * A214706  a(n) = a(n-1)*a(n-2) with a(0)=1, a(1)=5.
 * https://oeis.org/A214706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 8, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes any term in the OEIS A214706 sequence. */

#include <stdlib.h>

static seq_t *cache = NULL;     /* store computed terms */
static size_t cache_cap = 0;    /* number of terms available */

/* Ensure that cache contains terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;          /* already computed */

    /* Allocate or enlarge cache to hold up to n. */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)                    /* allocation failure is fatal */
        exit(EXIT_FAILURE);
    cache = tmp;

    /* Initialize the first two terms if cache was empty. */
    if (cache_cap == 0) {
        cache[0] = (seq_t)1;
        if (new_cap > 1)
            cache[1] = (seq_t)5;
        cache_cap = new_cap > 2 ? 2 : new_cap;
    }

    /* Compute any new terms using the recurrence. */
    for (size_t i = cache_cap; i < new_cap; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)5;
        else
            cache[i] = cache[i - 1] * cache[i - 2];
    }

    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
