/*
 * A067888  Numbers k such that tau(k+1) = tau(k-1) where tau(k) = A000005(k).
 * https://oeis.org/A067888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 9024, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A067888: numbers k such that τ(k‑1)=τ(k+1). Computed by brute‑force search. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/divisor_count_u64.h"

/* Cache for already found terms.  cache[0] holds a(1). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of entries currently stored          */
static idx_t cache_cap = 0;   /* capacity of the cache array                 */

/* The largest integer examined so far.  Start just before the first
   candidate 4 so that the first loop iteration checks k = 4.         */
static idx_t last_checked = 3;

/* Append a new element to the cache, growing the array if needed. */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n‑th term of the sequence (n ≥ 1).  The function is
   free of side effects except for the internal static cache. */
seq_t a(idx_t n)
{
    /* Assume n ≥ 1.  Return a cached value if it already exists. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Search forward until we have found the n‑th term. */
    while (cache_len < n) {
        ++last_checked;                       /* next candidate k   */

        uint64_t tau_left  = divisor_count_u64(last_checked - 1);
        uint64_t tau_right = divisor_count_u64(last_checked + 1);

        if (tau_left == tau_right)
            append_cache((seq_t)last_checked); /* store the qualifying k */
    }
    return cache[n - 1];
}
