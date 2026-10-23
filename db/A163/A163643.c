/*
 * A163643  a(0)=1. For n >= 1, a(n) is the smallest composite integer that is
 *          > a(n-1) and that is coprime to n.
 * https://oeis.org/A163643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1. For n >= 1, a(n) is the smallest composite integer >
 *            a(n-1) that is coprime to n
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   allocate_seq_array, gcd, is_prime_small, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependency utilities */
#include <stdbool.h>
#include "../utils/allocate_seq_array.h"
#include "../utils/seq_realloc.h"
#include "../utils/is_prime_small.h"
#include "../utils/gcd.h"

/* Cached sequence storage */
static seq_t *seq_cache = NULL;     /* array of computed values   */
static size_t cache_capacity = 0;   /* allocated length of array  */
static size_t cache_computed = 0;   /* next index to compute      */
static seq_t  cache_last = (seq_t)0;/* last computed value a(n-1)  */

seq_t a(idx_t n)
{
    /* Special case for the offset term */
    if (n == (idx_t)0) {
        return (seq_t)1;
    }

    /* Ensure the cache array exists and fits the requested index */
    size_t needed_cap = (size_t)n + 1;
    if (seq_cache == NULL) {
        /* First allocation */
        seq_cache = allocate_seq_array(needed_cap);
        cache_capacity = needed_cap;
    } else if (cache_capacity < needed_cap) {
        /* Grow the array if necessary */
        seq_t *tmp = seq_realloc(seq_cache, cache_capacity, needed_cap);
        seq_cache = tmp;
        cache_capacity = needed_cap;
    }

    /* Compute missing terms up to the requested index */
    if (cache_computed == 0) {      /* first computation: a(0) = 1 */
        seq_cache[0] = (seq_t)1;
        cache_last = seq_cache[0];
        cache_computed = 1;
    }

    for (size_t i = cache_computed; i <= (size_t)n; ++i) {
        idx_t cur_n = (idx_t)i;            /* current n for this term          */
        seq_t cand = cache_last + (seq_t)1;/* start searching after previous term */

        while (true) {
            /* Composite check: >1 and not prime */
            if (cand > (seq_t)1 && !is_prime_small(cand)) {
                seq_t sn = (seq_t)cur_n;   /* n as seq_t for gcd */
                if (gcd(cand, sn) == (seq_t)1) {
                    break; /* found the desired term */
                }
            }
            ++cand;                     /* try next integer */
        }

        seq_cache[i] = cand;
        cache_last   = cand;
    }

    cache_computed = (size_t)n + 1;
    return seq_cache[n];
}
