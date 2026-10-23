/*
 * A117205  Odd squarefree positive integers k such that (k+1)/2 is also
 *          squarefree.
 * https://oeis.org/A117205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS sequence A117205:
   odd square‑free positive integers k such that (k+1)/2 is also square‑free.
   The algorithm simply scans odd integers, tests the square‑free condition
   using the provided helper `is_squarefree`, and caches found terms.
   Complexity is O(n) for the n‑th term. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

static seq_t *cache       = NULL;  /* cached sequence values */
static size_t cache_len   = 0;      /* number of cached terms   */
static size_t cache_cap   = 0;      /* current allocation size  */
static seq_t next_candidate = 1;    /* next odd integer to test */

/* append a value to the cache, growing the array when needed */
static void cache_append(seq_t val) {
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0 ? 16 : cache_cap * 2);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Main sequence function: returns a(n) for any n ≥ 1, with arbitrary order of calls. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but keep safe */

    /* Ensure cache contains at least n terms */
    while (cache_len < (size_t)n) {
        if (is_squarefree(next_candidate)) {
            seq_t half = (next_candidate + 1) / 2;
            if (is_squarefree(half)) {
                cache_append(next_candidate);
            }
        }
        next_candidate += 2;   /* step to the next odd integer */
    }

    return cache[(size_t)n - 1];
}
