/*
 * A214195  Numbers with the number of distinct prime factors a multiple of 3.
 * https://oeis.org/A214195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers with the number of distinct prime factors a multiple of
 *            3 (computed by a sieve)
 * Range:     u8 n <= 44, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
# define ALLOC_SEQ(cap)  new seq_t[(cap)]
# define DEALLOC_SEQ(p)  delete [] (p)
#else
# define ALLOC_SEQ(cap)  ((seq_t*)calloc((cap), sizeof(seq_t)))
# define DEALLOC_SEQ(p)  free(p)
#endif

/* Static cache data */
static seq_t *cache = NULL;        /* array of sequence values          */
static size_t cache_len = 0;       /* how many values are stored       */
static size_t cache_cap = 0;       /* capacity of the allocated array   */
static size_t cache_limit = 0;     /* largest integer processed so far */

/* Build the cache up to the given integer limit (inclusive) */
static void rebuild_cache(size_t limit)
{
    /* 1. Count distinct prime factors for 1..limit */
    uint8_t *omega = (uint8_t*)calloc(limit + 1, 1);   /* zero‑initialised   */
    if (!omega) {
        /* In an unlikely memory‑failure scenario fall back to zero */
        free(omega);
        return;
    }

    for (size_t i = 2; i <= limit; ++i) {
        if (omega[i] == 0) {                 /* i is prime           */
            for (size_t j = i; j <= limit; j += i) {
                ++omega[j];
            }
        }
    }

    /* 2. Count how many numbers satisfy the condition */
    size_t count = 1;                       /* the number 1 (zero distinct primes) */
    for (size_t i = 2; i <= limit; ++i) {
        if (omega[i] % 3 == 0) ++count;
    }

    /* 3. Allocate a new cache array of the exact needed size */
    seq_t *new_cache = ALLOC_SEQ(count);
    if (!new_cache) {
        free(omega);
        return;
    }

    /* 4. Fill the array */
    size_t pos = 0;
    new_cache[pos++] = (seq_t)1;            /* first element is 1       */
    for (size_t i = 2; i <= limit; ++i) {
        if (omega[i] % 3 == 0) {
            new_cache[pos++] = (seq_t)i;
        }
    }

    /* 5. Free old structures and replace cache pointers */
    if (cache) DEALLOC_SEQ(cache);
    free(omega);

    cache = new_cache;
    cache_len = count;
    cache_cap = count;
    cache_limit = limit;
}

/* Ensure the cache contains at least n values (1‑based).          */
/* If it does not, the cache is rebuilt with a larger limit.      */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;       /* already sufficient          */

    size_t limit = (cache_limit > 0) ? cache_limit * 2 : (size_t)(n * 3 + 100);
    if (limit < 100) limit = 100;

    /* Increase the limit until we collect at least n terms.     */
    while (true) {
        rebuild_cache(limit);
        if (cache_len >= (size_t)n) break;
        limit *= 2;   /* double the limit and try again */
    }
}

/* Return the nth term of A214195 (offset 1).                       */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined, but harmless for callers */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
