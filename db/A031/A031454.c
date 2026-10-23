/*
 * A031454  Numbers whose base-3 representation has 2 more 0's than 2's.
 * https://oeis.org/A031454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   alloc_seq, free_seq, has_two_more_zeroes_than_twos
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = numbers whose base‑3 representation has two more 0's than 2's */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"
#include "../utils/has_two_more_zeroes_than_twos.h"

/* Global cache of found terms */
static seq_t *cache       = NULL; /* array of qualifying numbers */
static size_t cache_cap   = 0;    /* allocated capacity */
static size_t cache_len   = 0;    /* number of entries stored   */
static seq_t next_candidate = (seq_t)1; /* next integer to test */

/* Ensure the cache can store at least 'need' elements */
static inline void ensure_cache(size_t need)
{
    if (cache_len >= need) return;

    /* Grow capacity until it is large enough */
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;

    /* Allocate new array */
    seq_t *newp = alloc_seq(new_cap);
    if (!newp) return; /* allocation failed – the caller will not recover */

    /* Copy existing contents */
    for (size_t i = 0; i < cache_len; ++i) newp[i] = cache[i];

    /* Free old array and update pointers */
    if (cache) free_seq(cache);
    cache = newp;
    cache_cap = new_cap;
}

/* External‑linkage function computing the n‑th term (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset 0, handled gracefully */

    size_t target = (size_t)n;   /* safe cast: n >= 1 and fits into size_t */

    ensure_cache(target);

    /* Generate terms until we have at least n entries */
    while (cache_len < target) {
        if (has_two_more_zeroes_than_twos(next_candidate))
            cache[cache_len++] = next_candidate;
        ++next_candidate;
    }

    return cache[target - 1];
}
