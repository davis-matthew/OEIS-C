/*
 * A044286  Numbers k such that the string 3,8 occurs in the base 9
 *          representation of k but not of k-1.
 * https://oeis.org/A044286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_38
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that the string 3,8 occurs in the base‑9 representation of k
   but not of k−1.  The function computes the n‑th such number (n ≥ 1). The
   implementation uses a simple incremental search with a cache that is
   shared across calls. */

/* The following headers provide the required types and functions.  They are
   available in all supported builds. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_38.h"

/* Cached values of the sequence (0‑based index).  The cache grows as needed
   and is shared between calls. */
static seq_t *cache     = NULL;   /* array of terms */
static size_t cache_sz = 0;      /* number of terms currently stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* The integer that will be tested next.  Starts at 1 and increases with
   every iteration. */
static idx_t next_k = 1;

/* Ensure that the cache has space for at least new_cap elements.  The
   capacity is increased geometrically to guarantee amortised O(1) behaviour. */
static void ensure_cap(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t new_size = cache_cap ? cache_cap * 2 : 16;
    while (new_size < new_cap)
        new_size *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    /* In the unlikely event that realloc fails we abort; the build will
       terminate and the test harness will report a failure. */
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_size;
}

/* Return the n‑th term (n ≥ 1).  The sequence is memoised in the static
   cache; calls with out‑of‑order n are handled correctly. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for the problem; return a safe value */
        return 0;

    /* If already computed, return immediately. */
    if (n <= cache_sz)
        return cache[n - 1];

    /* Search further until we have at least n terms. */
    while (cache_sz < n) {
        seq_t cur = (seq_t)next_k;          /* current integer to test */
        seq_t prev = (seq_t)(next_k - 1);    /* previous integer */

        bool cur_has  = has_38(cur);
        bool prev_has = has_38(prev);

        if (cur_has && !prev_has) {
            /* Store the new term. */
            ensure_cap(cache_sz + 1);
            cache[cache_sz++] = cur;
        }

        next_k++;   /* next integer to consider */
    }

    return cache[n - 1];
}
