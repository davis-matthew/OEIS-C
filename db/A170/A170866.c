/*
 * A170866  a(0) = 1, a(1) = 1; a(2^i + j) = 2a(j) + 3a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 1,
 * a(2^i + j) = 2*a(j) + 3*a(j + 1)  for 0 <= j < 2^i */
#include <stdlib.h>
#include <stdint.h>

/* Memoised recursive evaluation of the sequence. */
static seq_t *cache = NULL;            /* cache of previously computed values */
static size_t cache_cap = 0;           /* number of slots currently allocated */

/* Extend the cache so that it can hold index n (inclusive).  New slots are
 * initialised to zero, which is a safe sentinel because all sequence values
 * are positive. */
static void ensure_cache_size(idx_t n)
{
    if (n < (idx_t)cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap << 1 : 2;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(1);                      /* allocation failure -- terminate */

    /* Initialise the new slots to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_cap = new_cap;
}

/* Recursive helper that returns a(n), using memoisation. */
static seq_t compute_rec(idx_t n)
{
    ensure_cache_size(n);
    size_t idx = (size_t)n;           /* array index */

    /* If already computed, return cached value. */
    if (cache[idx] != 0)
        return cache[idx];

    seq_t result;
    if (n == 0 || n == 1) {
        result = 1;
    } else {
        /* Find the largest power of two <= n.  For n >= 2 this is safe. */
        idx_t pow2 = 1;
        while (pow2 <= n)
            pow2 <<= 1;
        pow2 >>= 1;

        idx_t j = n - pow2;
        seq_t a_j  = compute_rec(j);
        seq_t a_j1 = compute_rec(j + 1);
        result = 2 * a_j + 3 * a_j1;
    }

    cache[idx] = result;
    return result;
}

/* Public interface for the OEIS sequence. */
seq_t
a(idx_t n)
{
    return compute_rec(n);
}
