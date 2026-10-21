/*
 * A218175  Number of nX2 arrays of the minimum value of corresponding
 *          elements and their horizontal and vertical neighbors in a random
 *          0..3 nX2 array.
 * https://oeis.org/A218175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 8*a(n-2) + 13*a(n-3) – 33*a(n-4) – 36*a(n-5) –
 *            86*a(n-6) – 145*a(n-7) – 161*a(n-8) + 132*a(n-9) + 538*a(n-10) +
 *            364*a(n-11) – 112*a(n-12) – 32*a(n-13) Initial values: a(1)=4,
 *            a(2)=28, a(3)=192, a(4)=956, a(5)=5494, a(6)=30888, a(7)=172038,
 *            a(8)=964828, a(9)=5400158, a(10)=302257...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   alloc_cache, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_cache.h"
#include "../utils/free_cache.h"

/* Static cache storage */
static seq_t *cache   = NULL;          /* array of computed values, index 0 unused   */
static size_t cache_len = 0;           /* number of allocated slots (max index+1)   */
static idx_t  computed  = 0;           /* highest index computed so far (>=0)      */

static const seq_t init_vals[13] = {
    4ULL,          /* a(1)  */
    28ULL,         /* a(2)  */
    192ULL,        /* a(3)  */
    956ULL,        /* a(4)  */
    5494ULL,       /* a(5)  */
    30888ULL,      /* a(6)  */
    172038ULL,     /* a(7)  */
    964828ULL,     /* a(8)  */
    5400158ULL,    /* a(9)  */
    30225794ULL,   /* a(10) */
    169223054ULL,  /* a(11) */
    947302656ULL,  /* a(12) */
    5303071898ULL  /* a(13) */
};

/* Allocate or grow the cache to hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 <= cache_len) return;

    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = alloc_cache(new_len);
    if (!new_cache) exit(1);        /* allocation failure */

    /* initialise newly allocated slots to zero */
    for (size_t i = cache_len; i < new_len; ++i) new_cache[i] = 0;

    /* copy old values, if any */
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];

    /* release old cache */
    if (cache != NULL) free_cache(cache);

    cache     = new_cache;
    cache_len = new_len;
}

/* Compute values from (computed+1) up to n using the recurrence */
static void compute_up_to(idx_t n)
{
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 13) {
            cache[i] = init_vals[i - 1];
        } else {
            seq_t sum_pos =
                4 * cache[i - 1] +
                8 * cache[i - 2] +
                13 * cache[i - 3] +
                132 * cache[i - 9] +
                538 * cache[i - 10] +
                364 * cache[i - 11];

            seq_t sum_neg =
                33 * cache[i - 4] +
                36 * cache[i - 5] +
                86 * cache[i - 6] +
                145 * cache[i - 7] +
                161 * cache[i - 8] +
                112 * cache[i - 12] +
                32 * cache[i - 13];

            cache[i] = sum_pos - sum_neg;
        }
    }
    computed = n;
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but keep safe */

    /* Ensure cache can accommodate the requested index */
    ensure_cache_size(n);

    /* Compute missing values */
    if (computed < n) compute_up_to(n);

    return cache[n];
}
