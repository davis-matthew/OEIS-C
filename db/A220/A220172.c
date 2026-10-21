/*
 * A220172  Sum of neighbor maps: number of n X 1 binary arrays indicating the
 *          locations of corresponding elements equal to the sum mod 3 of
 *          their horizontal, vertical and antidiagonal neighbors in a random
 *          0..2 n X 1 array.
 * https://oeis.org/A220172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 32, u64 n <= 63, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 4*a(n-3) - 8*a(n-4) - 4*a(n-6) + 8*a(n-7)
 * Initial values: a(1)=2, a(2)=2, a(3)=8, a(4)=16, a(5)=24, a(6)=64, a(7)=128
 * The recurrence holds for all n ≥ 8.
 */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stddef.h>   /* size_t */

static seq_t init_vals[8] = {
    (seq_t)0,   /* 0 (unused) */
    (seq_t)2,   /* a(1) */
    (seq_t)2,   /* a(2) */
    (seq_t)8,   /* a(3) */
    (seq_t)16,  /* a(4) */
    (seq_t)24,  /* a(5) */
    (seq_t)64,  /* a(6) */
    (seq_t)128  /* a(7) */
};

static seq_t *cache      = NULL;   /* cache[1] .. cache[max] */
static size_t cache_cap  = 0;       /* total allocated capacity */
static idx_t   cache_max = 0;       /* largest index computed */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)n + 1) {
        size_t newcap = cache_cap ? cache_cap : 8;
        while (newcap < (size_t)n + 1)
            newcap <<= 1;  /* double until big enough */

        seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!newptr) {
            /* Allocation failed – terminate the program. */
            abort();
        }
        cache = newptr;
        cache_cap = newcap;
    }
}

/* Compute a(idx_t n) for A220172. */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for the sequence, but return 0 to be safe */
        return (seq_t)0;

    /* Handle indices 1..7 directly. */
    if (n <= 7)
        return init_vals[n];

    /* Prepare the cache. */
    ensure_cache(n);

    /* Initialize the cache with the first seven values if necessary. */
    if (cache_max < 7) {
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = init_vals[i];
        cache_max = 7;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t t6 = cache[i - 6];
        seq_t t7 = cache[i - 7];
        cache[i] = 2 * t1 + 4 * t3 - 8 * t4 - 4 * t6 + 8 * t7;
    }

    cache_max = n;
    return cache[n];
}
