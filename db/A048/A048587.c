/*
 * A048587  Pisot sequence L(6,10).
 * https://oeis.org/A048587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - a(n-4) with initial
 *            values a(0)=6, a(1)=10, a(2)=17, a(3)=29
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 36, u64 n <= 75, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

static seq_t *seq_cache = NULL;   /* cached values */
static size_t cache_len = 0;      /* number of valid entries */
static size_t cache_cap = 0;      /* allocated capacity */

/* Ensure the cache holds at least `need` entries (0‑based). */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* return the initial terms directly if requested */
    if (n == 0) return 6;
    if (n == 1) return 10;
    if (n == 2) return 17;
    if (n == 3) return 29;

    /* initialize cache with the first four terms if needed */
    if (!seq_cache) {
        ensure_cache_capacity(4);
        seq_cache[0] = 6;
        seq_cache[1] = 10;
        seq_cache[2] = 17;
        seq_cache[3] = 29;
        cache_len = 4;
    }

    /* if the requested term is already cached, return it */
    if ((size_t)n < cache_len) return seq_cache[n];

    /* grow cache to hold terms up to n */
    ensure_cache_capacity((size_t)n + 1);

    /* compute missing terms */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t p1 = 3 * seq_cache[i-1] + 2 * seq_cache[i-3];
        seq_t p2 = 3 * seq_cache[i-2] + seq_cache[i-4];
        seq_cache[i] = p1 - p2;
    }
    cache_len = (size_t)n + 1;

    return seq_cache[n];
}
