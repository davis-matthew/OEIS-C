/*
 * A118012  a(n) = 4*A117384(n) - n; a self-inverse permutation of the natural
 *          numbers.
 * https://oeis.org/A118012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*A117384(n) - n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the self‑inverse permutation A118012.
   The algorithm keeps a cache of already computed values and
   generates new pairs (i, j) where j = 4*s - i with s the next
   unused value of A117384.  Each pair is produced exactly once.  */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#ifdef __cplusplus
#  include <new>
#endif

/* External types defined by the build system */
extern seq_t a(idx_t n);

/* Cache for the permutation values (1‑based).  */
static seq_t *cache = NULL;          /* array of length cache_cap */
static size_t cache_cap = 0;          /* actual allocated size */

/* Next unused integer index.  */
static idx_t next_unused_start = 1;
/* Next value of A117384 to use when forming a new pair.  */
static idx_t next_m = 1;

/* Ensure that the cache has at least ``min_cap`` elements.  */
static void
ensure_cache_cap(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;

    /* Grow cache by powers of two for amortized linear time. */
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < min_cap)
        new_cap <<= 1;

#ifdef __cplusplus
    /* Allocate a new array with default construction. */
    seq_t *new_ptr = new seq_t[new_cap];
    if (cache) {
        /* Copy existing values. */
        for (size_t i = 0; i < cache_cap; ++i)
            new_ptr[i] = cache[i];
        delete[] cache;          /* free old array */
    }
    /* All new elements are default‑constructed to zero. */
    cache = new_ptr;
#else
    /* Use realloc and zero‑initialize new part. */
    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    if (!cache)
        memset(tmp, 0, new_cap * sizeof(seq_t));
    else
        memset(tmp + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = tmp;
#endif
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* formal guard – sequence is 1‑based */

    /* Allocate space for the requested index. */
    ensure_cache_cap((size_t)n + 1);

    /* Generate new pairs until all indices ≤ n are filled. */
    for (;;) {
        /* Skip over already used indices. */
        while (next_unused_start < cache_cap && cache[next_unused_start] != (seq_t)0)
            ++next_unused_start;

        /* If the smallest unused index exceeds n, we are done. */
        if (next_unused_start > n)
            break;

        /* Form the new pair (i, j).  i is the smallest unused integer. */
        idx_t i = next_unused_start;
        seq_t sum = (seq_t)4 * (seq_t)next_m;     /* 4*s */
        idx_t j = (idx_t)(sum - (seq_t)i);       /* j = 4*s - i */

        /* Make sure the cache is large enough for j. */
        ensure_cache_cap((size_t)j + 1);

        /* Store the pair.  The partner j cannot have been assigned yet. */
        cache[i] = (seq_t)j;
        cache[j] = (seq_t)i;

        /* Advance pointers for the next iteration. */
        ++next_unused_start;  /* i is now used */
        ++next_m;
    }

    return cache[n];
}
