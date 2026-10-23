/*
 * A175759  Numbers n such that bigomega( n+bigomega(n) ) = bigomega(n), where
 *          bigomega = A001222.
 * https://oeis.org/A175759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   alloc_seq_array, bigomega, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175759:
   a(n) is the n‑th integer such that bigomega( n + bigomega(n) ) == bigomega(n).
   This implementation scans successive integers, keeps those that satisfy the
   condition, and caches them for fast arbitrary‑order queries. */

#include <stdlib.h>
#include "../utils/bigomega.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *term_cache = NULL;   /* 1‑based cache of sequence terms   */
static size_t term_cache_size = 0; /* number of elements stored         */
static size_t term_cache_cap  = 0; /* allocated capacity                */
static idx_t  last_checked     = 0;/* last integer examined             */

/* enlarge the cache to at least `new_cap` elements */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= term_cache_cap) return;
    size_t new_capacity = term_cache_cap ? term_cache_cap * 2 : 16;
    while (new_capacity < new_cap) new_capacity *= 2;
    seq_t *tmp = alloc_seq_array(new_capacity);
    for (size_t i = 0; i < term_cache_size; ++i)
        tmp[i] = term_cache[i];
    if (term_cache) free_seq_array(term_cache);
    term_cache = tmp;
    term_cache_cap = new_capacity;
}

/* ensure that the cache contains at least `n` terms */
static void ensure_terms(size_t n)
{
    while (term_cache_size < n)
    {
        idx_t candidate = last_checked + 1;
        seq_t omega_candidate = bigomega(candidate);
        idx_t c_plus = candidate + (idx_t)omega_candidate;
        seq_t omega_sum = bigomega(c_plus);
        if (omega_sum == omega_candidate)
        {
            grow_cache(term_cache_size + 1);
            term_cache[term_cache_size++] = candidate;
        }
        last_checked = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined; safe to return 0 */
    ensure_terms((size_t)n);
    return term_cache[(size_t)n - 1];
}
