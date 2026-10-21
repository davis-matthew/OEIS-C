/*
 * A217393  Smallest k > 0 such that 1 + n^2 and 1 + (n+k)^2 have the same
 *          largest factor, or 0 if no such k exists.
 * https://oeis.org/A217393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that the largest prime factor of
 *            n^2+1 equals the largest prime factor of (n+k)^2+1, or 0 if no
 *            such k exists. Computed by pre‑computing the largest prime
 *            factors up to the needed index and scanning backwards while
 *            remembering the next occurrence of each factor
 * Range:     u8 n <= 19, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/largest_prime_factor.h"

/* Typedefs provided by the build system */
extern seq_t a(idx_t n);  /* forward declaration */

/* --- Static cache state --------------------------------------------------- */
static idx_t cache_max = 0;          /* largest n for which caches are valid */

/* For values n^2+1 the largest prime factor always fits in 64 bits
   when n ≤ 10 000 (and even larger indices for the test harness),
   so we store it as uint64_t. */
static uint64_t *gpf_cache = NULL;   /* largest prime factor of n^2+1 */

/* The sequence values themselves may be arbitrary precision, so we keep
   them exactly in seq_t. */
static seq_t *a_cache   = NULL;      /* result a(n) */

/* ------------------------------------------------------------------------- */
static void recompute(idx_t limit)
{
    /* Allocate new caches */
    uint64_t *new_gpf = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    seq_t    *new_a   = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!new_gpf || !new_a) {
        /* In a real implementation we might abort; here we just exit */
        abort();
    }

    /* 1. Compute the largest prime factor for each 1 ≤ n ≤ limit. */
    for (idx_t i = 1; i <= limit; ++i) {
        uint64_t val = (uint64_t)i * (uint64_t)i + 1ULL;
        new_gpf[i] = largest_prime_factor(val);
    }

    /* 2. Scan from the end to the beginning using an open‑addressing map
       to remember the most recent index where a given factor appeared. */
    size_t map_cap = 1;
    while (map_cap < (size_t)(limit * 2)) map_cap <<= 1;  /* load factor ~0.5 */
    typedef struct { uint64_t key; idx_t val; } map_entry;
    map_entry *map = (map_entry *)calloc(map_cap, sizeof(map_entry));
    if (!map) abort();
    size_t mask = map_cap - 1;

    for (idx_t i = limit; i >= 1; --i) {
        uint64_t key = new_gpf[i];
        size_t pos = key & mask;
        while (1) {
            if (map[pos].key == 0) {                     /* first time */
                new_a[i] = (seq_t)0;
                map[pos].key = key;
                map[pos].val = i;
                break;
            } else if (map[pos].key == key) {            /* seen before */
                idx_t next = map[pos].val;
                new_a[i] = (seq_t)(next - i);
                map[pos].val = i;                        /* update for earlier n */
                break;
            } else {                                     /* hash probe */
                pos = (pos + 1) & mask;
            }
        }
    }

    /* Free old caches */
    free(gpf_cache);
    free(a_cache);
    free(map);

    /* Store the new ones */
    gpf_cache = new_gpf;
    a_cache   = new_a;
    cache_max = limit;
}

/* Ensure that caches are available up to at least 'needed'.  We grow a bit
   beyond the requested limit to reduce the number of reallocations. */
static void ensure_cache(idx_t needed)
{
    if (needed <= cache_max) return;
    recompute(needed + 5000);   /* extra buffer */
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence is undefined for n=0 */
    ensure_cache(n);
    return a_cache[n];
}
