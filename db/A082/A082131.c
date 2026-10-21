/*
 * A082131  Middle of semiprime triple: n-2, n, n+2 are semiprimes.
 * https://oeis.org/A082131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 1376, u32 n <= 9943, u64 n <= 9943,
 *            u128 n <= 9943, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A082131: a(n) is the n‑th odd integer whose neighbors are all
   semiprimes (i.e. n‑2, n, n+2 have exactly two prime factors,
   counting multiplicity).  The function computes the value by
   generating candidates in ascending order, using bigomega() to test
   semiprimality. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bigomega.h"

static inline bool is_semiprime_idx(idx_t x)
{
    /* bigomega(x) returns the total number of prime factors of x
       (counted with multiplicity).  It returns 0 for x == 1.
       An integer is a semiprime iff this count equals 2. */
    return bigomega(x) == 2;
}

/* Cached sequence and its bookkeeping state.  All variables are
   initialization‑performed on first use. */
static seq_t  *cache      = NULL;   /* array of computed values   */
static size_t  cache_cap  = 0;      /* allocated capacity         */
static size_t  cache_len  = 0;      /* number of values stored    */
static idx_t   next       = 3;      /* next odd integer to test   */

/* Resize the backing array to at least new_cap elements.
   The implementation is aware of C and C++ differences. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;

    seq_t *tmp = NULL;
#ifdef __cplusplus
    /* In C++ use new[] and delete[]; the class will be constructed
       and destructed automatically. */
    tmp = new seq_t[new_cap];
    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    delete[] cache;
#else
    /* In C use realloc; cast the void* result to the correct type. */
    tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();     /* out of memory */
#endif
    cache      = tmp;
    cache_cap  = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n==0, but safe */

    /* Ensure the cache has enough space to hold n terms.  Start with a
       small capacity to avoid unnecessary allocations. */
    if (cache_len == 0)
        ensure_capacity(4);

    /* Grow the array if more space is needed. */
    while (cache_len < (size_t)n)
    {
        /* Find the next odd integer whose neighbors are semiprimes. */
        while (true)
        {
            if (is_semiprime_idx(next - 2) &&
                is_semiprime_idx(next)     &&
                is_semiprime_idx(next + 2))
            {
                break;                 /* found a valid middle */
            }
            next += 2;                 /* skip to next odd */
        }

        /* Store the found term in the cache. */
        if (cache_len == cache_cap)
            ensure_capacity(cache_cap * 2); /* double capacity */

        cache[cache_len++] = (seq_t)next;

        /* Skip the even number following the middle to keep searching
           only odd candidates. */
        next += 2;
    }

    return cache[n - 1];
}
