/*
 * A188915  Union of squares and powers of 2.
 * https://oeis.org/A188915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 262, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Union of squares and powers of 2 (A188915) */
/* a(n) = the n‑th element of the sorted union of {k²: k≥0} and {2^k: k≥0} */
#include <stdlib.h>
#include "../utils/pow2.h"

/* public function ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a static cache that is lazily filled up to the maximum requested
       index.  The cache is independent of the order of calls. */
    static seq_t *cache   = NULL;   /* stores the sequence values          */
    static size_t cache_len  = 0;   /* number of values stored so far      */
    static size_t cache_cap  = 0;   /* allocated capacity                  */

    /* state of the two generating streams */
    static idx_t  sq_idx  = 0;          /* current square index k           */
    static seq_t  sq_val  = 0;          /* current square value k²           */
    static idx_t  pow_idx = 0;          /* current power‑of‑two exponent k   */
    static seq_t  pow_val = 1;          /* current power‑of‑two value 2^k    */

    /* initialise the cache on first call */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) abort();  /* memory allocation failed */
    }

    /* expand the cache until it covers the requested index n */
    while (cache_len <= n) {
        /* resize the cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if (new_cap < cache_len + 1) new_cap = cache_len + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();           /* memory allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* pick the smaller of the two streams (ties are taken once) */
        seq_t next = (sq_val <= pow_val) ? sq_val : pow_val;
        cache[cache_len++] = next;

        /* advance any stream that produced the chosen value */
        if (sq_val == next) {
            /* advance squares: (k+1)² */
            sq_idx++;
            sq_val = (seq_t)sq_idx * (seq_t)sq_idx;
        }
        if (pow_val == next) {
            /* advance powers of two: 2^(k+1) */
            pow_idx++;
            pow_val = pow2(pow_idx);
        }
    }

    return cache[n];
}
