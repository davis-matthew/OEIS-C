/*
 * A218420  Number of n X 2 binary arrays with every 0 a horizontal, vertical
 *          or antidiagonal neighbor to some 1.
 * https://oeis.org/A218420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) + a(n-3) + a(n-4) + a(n-5) with
 *            a(1)=3, a(2)=13, a(3)=49, a(4)=191, a(5)=737 (1‑based)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[0] = a(1) */
static size_t cache_len = 0;         /* number of computed terms */
static size_t cache_cap = 0;         /* allocated capacity */

/* Ensure that the cache contains at least n terms (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    if (n <= (idx_t)cache_len)
        return;

    size_t needed = (size_t)n;

    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort; the program will terminate. */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize new entries */
    for (size_t i = cache_len; i < needed; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)3;
        } else if (i == 1) {
            cache[i] = (seq_t)13;
        } else if (i == 2) {
            cache[i] = (seq_t)49;
        } else if (i == 3) {
            cache[i] = (seq_t)191;
        } else if (i == 4) {
            cache[i] = (seq_t)737;
        } else {
            cache[i] = (seq_t)3 * cache[i - 1] +
                       (seq_t)3 * cache[i - 2] +
                       cache[i - 3] +
                       cache[i - 4] +
                       cache[i - 5];
        }
    }
    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* below offset – value undefined */

    ensure_cache(n);
    return cache[(size_t)n - 1];
}
