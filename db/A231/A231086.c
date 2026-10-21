/*
 * A231086  Initial members of abundant twins, i.e., values of k such that (k,
 *          k+2) are both abundant numbers.
 * https://oeis.org/A231086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1890, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th member of OEIS sequence A231086:  
   a(n) is the n-th integer k such that both k and k+2 are abundant. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Test whether n is an abundant number:
   abundant(n) ⇔ σ(n) > 2·n  */
static inline bool abundant(idx_t n)
{
    seq_t s = sigma_of(n);
    seq_t two_n = (seq_t)n * 2;
    return s > two_n;
}

/* Dynamic cache of sequence values.  Only grown, never shrunk. */
static seq_t *cache   = NULL;
static size_t cap     = 0;
static size_t len     = 0;
static uint64_t last_checked = 0;

/* Grow the cache to hold at least min_cap elements.  New
   entries are moved from the old array to the new one. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cap) return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < min_cap) new_cap <<= 1;

    seq_t *new_cache = alloc_seq_array(new_cap);
    if (!new_cache) abort();          /* out of memory */

    for (size_t i = 0; i < len; ++i)
        new_cache[i] = cache[i];

    if (cache) free_seq_array(cache);
    cache = new_cache;
    cap = new_cap;
}

/* Return the n‑th element of A231086 (n ≥ 1).  Calls may arrive in
   any order; previously computed terms are memoised. */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n terms. */
    while (len < (size_t)n) {
        ++last_checked;                           /* candidate k */
        if (abundant(last_checked) && abundant(last_checked + 2)) {
            grow_cache(len + 1);
            cache[len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
