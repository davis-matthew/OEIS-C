/*
 * A039606  Numbers whose base-12 representation has the same nonzero number
 *          of 1's and 4's.
 * https://oeis.org/A039606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth integer whose base‑12 representation contains *
 *            the same non‑zero number of 1's and 4's. Computed by a simple
 *            search
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cache of computed terms */
static seq_t *cache = NULL;          /* array of a(1), a(2), ... */
static idx_t  cache_size = 0;        /* number of terms currently cached */
static size_t cache_cap  = 0;        /* capacity of the cache array */

/* Ensure the cache has space for at least 'needed' elements. */
static void
ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();                       /* insufficient memory */

    for (size_t i = 0; i < cache_size; ++i)
        new_arr[i] = cache[i];

    free(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

/* Test whether a decimal integer has the same non‑zero number of 1's and 4's
 * in its base‑12 representation. */
static bool
has_equal_1_and_4(seq_t x)
{
    unsigned c1 = 0, c4 = 0;
    while (x > 0) {
        uint64_t d = (uint64_t)(x % 12);
        if (d == 1)
            ++c1;
        else if (d == 4)
            ++c4;
        x /= 12;
    }
    return (c1 == c4) && (c1 > 0);
}

/* Sequence A039606.
 * n is 1‑based.  For n == 0, return 0. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Start checking from the first integer after the last cached term. */
    seq_t cand = (cache_size == 0) ? (seq_t)1
                                   : cache[cache_size - 1] + (seq_t)1;

    while (cache_size < n) {
        if (has_equal_1_and_4(cand)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = cand;
        }
        cand++;
        /* No need to guard against overflow for the problem range. */
    }

    return cache[n - 1];
}
