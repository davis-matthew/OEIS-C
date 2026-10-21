/*
 * A221599  Number of 0..n arrays of length 7 with each element differing from
 *          at least one neighbor by 1 or less.
 * https://oeis.org/A221599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4) for n≥8, with the
 *            initial values a(1)=128, a(2)=1113, a(3)=4240, a(4)=11011,
 *            a(5)=22912, a(6)=41401, a(7)=67936
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for already computed terms */
static seq_t *cache      = NULL;   /* array of terms, 0‑based: cache[i] = a(i+1) */
static size_t cache_cap  = 0;      /* current allocated capacity (in terms) */
static idx_t   cache_size = 0;     /* number of terms stored (>= cache_cap?) */

/* Initialize the cache with the first 7 values. */
static void init_cache(void)
{
    const size_t init_cap = 7;                 /* at least up to a(7) */
    cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
    if (!cache) abort();                       /* allocation failed */

    cache[0] = (seq_t)128;                     /* a(1) */
    cache[1] = (seq_t)1113;                    /* a(2) */
    cache[2] = (seq_t)4240;                    /* a(3) */
    cache[3] = (seq_t)11011;                   /* a(4) */
    cache[4] = (seq_t)22912;                   /* a(5) */
    cache[5] = (seq_t)41401;                   /* a(6) */
    cache[6] = (seq_t)67936;                   /* a(7) */

    cache_cap  = init_cap;
    cache_size = init_cap;
}

/* Grow the cache to at least `need` terms (1‑based index). */
static void grow_cache(idx_t need)
{
    if (need <= (idx_t)cache_cap) return;

    size_t new_cap = cache_cap;
    while (new_cap < (size_t)need) new_cap <<= 1; /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                          /* allocation failed */
    cache     = tmp;
    cache_cap = new_cap;
}

/* Function required by the build.  Computes a(n).  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for n=0, return 0 */

    if (!cache) init_cache();                    /* make sure cache exists */

    /* Compute missing terms up to n if necessary. */
    if (n > cache_size) {
        /* Ensure we have enough allocated space. */
        grow_cache(n);

        for (idx_t i = cache_size; i < n; ++i) {
            /* Terms are 0‑based in cache, so cache[i-1] = a(i). */
            seq_t a1 = cache[i-1];   /* a(i)   = a(n-1) when i = n-1 */
            seq_t a2 = cache[i-2];   /* a(i-1) = a(n-2) */
            seq_t a3 = cache[i-3];   /* a(i-2) = a(n-3) */
            seq_t a4 = cache[i-4];   /* a(i-3) = a(n-4) */

            /* Apply the recurrence: a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) - a(n-4). */
            cache[i] = a1 * 4 - a2 * 6 + a3 * 4 - a4;
        }
        cache_size = n;
    }

    return cache[n-1];  /* cache[n-1] holds a(n) */
}
