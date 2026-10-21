/*
 * A206981  Number of n X 2 0..1 arrays avoiding the patterns 0 1 0 or 1 0 1
 *          in any row, column, diagonal or antidiagonal.
 * https://oeis.org/A206981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3) (n ≥ 4) Initial values:
 *            a(1)=4, a(2)=16, a(3)=36 (The recurrence holds for all n ≥ 4.)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;  /* array of computed terms, 1‑based indexing */
static size_t stored_len = 0;/* number of terms actually stored in cache   */
static size_t capacity = 0;  /* allocated size of cache                      */

seq_t a(idx_t n)
{
    if (n < 1)  /* offset is 1; behaviour below is irrelevant */
        return (seq_t)0;

    /* Initialise cache with the first three terms if it is the first call */
    if (cache == NULL) {
        capacity = 3;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL)  /* allocation failure: abort */
            exit(1);
        cache[0] = (seq_t)4;      /* a(1) */
        cache[1] = (seq_t)16;     /* a(2) */
        cache[2] = (seq_t)36;     /* a(3) */
        stored_len = 3;
    }

    if ((size_t)n <= stored_len)            /* already computed value */
        return cache[(size_t)n - 1];

    /* Grow the cache if necessary */
    if ((size_t)n > capacity) {
        size_t new_cap = capacity ? capacity : 1;
        while (new_cap < (size_t)n)
            new_cap <<= 1;                 /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)                 /* allocation failure: abort */
            exit(1);
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = stored_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1)
            val = (seq_t)4;
        else if (i == 2)
            val = (seq_t)16;
        else if (i == 3)
            val = (seq_t)36;
        else
            val = (seq_t)2 * cache[(size_t)i - 2]   /* a(i-1)   */
                + (seq_t)2 * cache[(size_t)i - 3]   /* a(i-2)   */
                - cache[(size_t)i - 4];            /* a(i-3)   */
        cache[(size_t)i - 1] = val;
    }
    stored_len = (size_t)n;
    return cache[(size_t)n - 1];
}
