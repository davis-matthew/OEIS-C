/*
 * A025940  Expansion of 1/((1-2x)(1-3x)(1-6x)(1-10x)).
 * https://oeis.org/A025940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*a(n-1) - 146*a(n-2) + 396*a(n-3) - 360*a(n-4) with
 *            a(0)=1, a(1)=21, a(2)=295, a(3)=3525 (valid for all n ≥ 0)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;    /* array holding previously computed terms   */
static size_t cache_len = 0;   /* number of terms currently stored          */
static size_t cache_cap = 0;   /* allocated capacity of the array           */

/* Allocate the initial cache containing the first four terms. */
static void init_cache(void)
{
    if (cache_len > 0)
        return; /* already initialised */

    cache_cap = 8;                      /* modest starting size */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        exit(EXIT_FAILURE);             /* allocation failed */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)21;
    cache[2] = (seq_t)295;
    cache[3] = (seq_t)3525;
    cache_len = 4;
}

/* Ensure the cache can hold at least *needed* elements. */
static void grow_cache(size_t needed)
{
    if (needed < cache_cap)
        return;                        /* already sufficient */

    size_t new_cap = cache_cap;
    if (new_cap == 0)
        new_cap = 8;
    while (new_cap <= needed)
        new_cap <<= 1;                  /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);             /* allocation failed */

    cache = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) using the 4‑term linear recurrence with memoisation. */
seq_t a(idx_t n)
{
    init_cache();

    /* If the value is already cached, return it immediately. */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Extend the cache up to and including index n. */
    size_t required = (size_t)n + 1;
    grow_cache(required);

    /* Compute the missing terms one by one. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t t1 = (seq_t)21   * cache[i - 1];
        seq_t t2 = (seq_t)146  * cache[i - 2];
        seq_t t3 = (seq_t)396  * cache[i - 3];
        seq_t t4 = (seq_t)360  * cache[i - 4];
        cache[i] = t1 - t2 + t3 - t4;
    }

    cache_len = required;
    return cache[(size_t)n];
}
