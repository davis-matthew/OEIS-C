/*
 * A159560  Minimal recursive sequence beginning with 3 such that
 *          A001221(a(n)) = A001221(n).
 * https://oeis.org/A159560
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* C implementation of OEIS A159560
 *
 * Definition:
 *   a(2) = 3
 *   a(n) = min{ m > a(n-1) : A001221(m) = A001221(n) }   for n ≥ 3
 * where A001221(n) is the number of distinct prime factors (ω(n)).
 *
 * The function works for arbitrary integer types seq_t and idx_t
 * (supplied by the build system).  It memoises computed values
 * so that calls are fast even when the indices are provided in
 * arbitrary order.
 *
 * Only <stdint.h>, <stdlib.h> and <stdbool.h> are required.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_distinct_primes_uint64.h"

/* --- Memoisation infrastructure ------------------------------------- */

static seq_t *cache      = NULL; /* cache[i] stores a(i) for i ≥ 2        */
static idx_t  cache_cap  = 0;    /* allocated capacity                      */
static idx_t  computed_up_to = 1;/* highest index already computed          */

/* Grow the cache array and compute all missing a(k) up to `up_to`. */
static void
ensure_cache(idx_t up_to)
{
    if (up_to <= computed_up_to) return;

    /* Extend capacity: at least `up_to + 1` (index 0/1 unused). */
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= (size_t)up_to) new_cap <<= 1;
    if (new_cap != cache_cap) {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* initialise new slots to 0 – safe for all seq_t types */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute missing entries. */
    for (idx_t i = computed_up_to + 1; i <= up_to; ++i) {
        if (i == 2) {
            cache[2] = (seq_t)3;
            continue;
        }
        /* ω(i) is the target value. */
        seq_t target = count_distinct_primes_uint64(i);

        /* Scan upwards from a(i-1)+1 until a matching ω is found. */
        idx_t m = (idx_t)cache[i - 1] + 1;
        while (count_distinct_primes_uint64(m) != target) {
            ++m;
        }
        cache[i] = (seq_t)m;
    }
    computed_up_to = up_to;
}

/* --- Public entry point ---------------------------------------------- */

/* Return a(n) for n ≥ 2.  Behaviour for n < 2 is undefined. */
seq_t
a(idx_t n)
{
    if (n < 2) return 0;
    ensure_cache(n);
    return cache[n];
}
