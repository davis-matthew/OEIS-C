/*
 * A022825  a(n) = a([ n/2 ]) + a([ n/3 ]) + . . . + a([ n/n ]) for n > 1,
 *          a(1) = 1.
 * https://oeis.org/A022825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=2..n} a(⌊n/k⌋), a(1) = 1
 * Range:     u8 n <= 59, u16 n <= 1439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises all values up to the largest n seen.
 * It uses a dynamic array that grows as required and works for any
 * order of calls.  Complexity is O(n²) in the worst case but fast
 * enough for n ≈ 10 000. */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

/* the build provides the following types */
 /* idx_t and seq_t are defined in the build configuration, e.g.:
  * typedef uint64_t idx_t;
  * typedef uint8_t  seq_t;   or other integer / big‑int type
  */

/* internal memoisation table */
static seq_t *cache = NULL;
static size_t cap   = 0;  /* next free index in cache (cache[i] valid for i < cap) */

/* Ensure cache has room for indices up to n (inclusive).  Returns the
 * previous cap value, i.e. the first index that needs computation. */
static size_t _ensure_cache(size_t n)
{
    if (n + 1 <= cap)                  /* already large enough */
        return cap;
    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                /* allocation failure */
    cache = new_cache;
    size_t old_cap = cap;
    cap = new_cap;
    return old_cap;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0)                     /* out of bounds; not used by OEIS */
        return (seq_t)0;
    size_t idx = (size_t)n;         /* safe within typical use range */

    /* initialise cache on first call */
    if (!cache) {
        cap = 2;                    /* indices 0 and 1 needed */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache)
            abort();
        cache[1] = (seq_t)1;        /* a(1) = 1, cache[0] stays 0 */
    }

    size_t start = _ensure_cache(idx);  /* first index that must be filled */

    /* compute missing values */
    for (size_t i = start; i <= idx; ++i) {
        seq_t sum = 0;
        /* k runs from 2 to i, floor(i/k) is at least 1 */
        for (size_t k = 2; k <= i; ++k) {
            size_t fk = i / k;
            sum += cache[fk];
        }
        cache[i] = sum;
    }

    return cache[idx];
}
