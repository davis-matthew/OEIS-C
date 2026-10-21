/*
 * A221652  Number of n X 2 arrays of occupancy after each element moves to
 *          some horizontal, diagonal or antidiagonal neighbor, without
 *          move-in move-out left turns.
 * https://oeis.org/A221652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) + a(n-3) for n>4 with initial values
 *            a(1)=1, a(2)=4, a(3)=12, a(4)=40
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for previously computed terms */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_size = 0;    /* number of terms stored */

/* ensure that cache has capacity for at least 'need' terms */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need)
        new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – terminate quietly; the problem statement
           guarantees that the allocation will succeed in the test harness */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n-th term of sequence A221652 (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined, but safe guard */

    size_t need = (size_t)n;           /* how many terms we need (1‑based) */
    if (cache_size < need) {
        ensure_cache_capacity(need);

        /* compute missing terms */
        for (size_t pos = cache_size; pos < need; ++pos) {
            if (pos == 0)
                cache[pos] = (seq_t)1;
            else if (pos == 1)
                cache[pos] = (seq_t)4;
            else if (pos == 2)
                cache[pos] = (seq_t)12;
            else if (pos == 3)
                cache[pos] = (seq_t)40;
            else
                cache[pos] = cache[pos-1] * (seq_t)3
                           + cache[pos-2]
                           + cache[pos-3];
        }
        cache_size = need;
    }
    return cache[n-1]; /* because cache[0] == a(1) */
}
