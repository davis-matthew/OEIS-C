/*
 * A221511  Number of 0..4 arrays of length n with each element differing from
 *          at least one neighbor by 2 or more, starting with 0.
 * https://oeis.org/A221511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) +6*a(n-2) +6*a(n-3) +4*a(n-4) +4*a(n-6);
   Initial values: a(1)=0, a(2)=3, a(3)=7, a(4)=36, a(5)=130, a(6)=532
 */

#include <stdlib.h>

static const seq_t BASE[7] = {
    0,     /* dummy for index 0 */
    0,     /* a(1) */
    3,     /* a(2) */
    7,     /* a(3) */
    36,    /* a(4) */
    130,   /* a(5) */
    532    /* a(6) */
};

static seq_t *cache = NULL;      /* memoization table, 1-indexed */
static size_t cache_size = 0;    /* allocated size of cache (including index 0) */
static idx_t  last_computed = 0; /* highest n that has been computed in cache */

/* Ensure cache has space for indices up to n (inclusive) */
static void ensure_cache_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* +1 because we use 1-based indexing */
    if (needed <= cache_size) return;

    seq_t *new_cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!new_cache) {
        /* handle allocation failure – abort as computation cannot continue */
        abort();
    }
    cache = new_cache;
    cache_size = needed;
}

seq_t a(idx_t n)
{
    if (n <= 6) return BASE[(size_t)n];

    /* First time: initialize cache and store base values */
    if (!cache) {
        cache = NULL;                          /* ensure realloc behaves like malloc */
        cache_size = 0;
    }

    /* Ensure we can address cache[n] */
    if ((size_t)n + 1 > cache_size) {
        ensure_cache_capacity(n);
    }

    /* Make sure base terms 1..6 are present */
    if (last_computed < 6) {
        for (idx_t i = 1; i <= 6; ++i) {
            cache[i] = BASE[(size_t)i];
        }
        last_computed = 6;
    }

    /* Compute recurrence for any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = 2*a(i-1) +6*a(i-2) +6*a(i-3) +4*a(i-4) +4*a(i-6) */
        seq_t res = cache[i - 1] * (seq_t)2;
        res += cache[i - 2] * (seq_t)6;
        res += cache[i - 3] * (seq_t)6;
        res += cache[i - 4] * (seq_t)4;
        res += cache[i - 6] * (seq_t)4;
        cache[i] = res;
    }

    last_computed = n;
    return cache[n];
}
