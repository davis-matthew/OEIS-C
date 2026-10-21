/*
 * A099242  (6n+5)-th terms of expansion of 1/(1 - x - x^6).
 * https://oeis.org/A099242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A099242 – (6n+5)-th terms of the expansion of 1/(1‑x‑x⁶)
 *
 *  Recurrence (valid for n ≥ 6):
 *      a(0) = 1,  a(1) = 7,  a(2) = 34,  a(3) = 153, a(4) = 686, a(5) = 3088
 *      a(n) = 7*a(n‑1) – 15*a(n‑2) + 20*a(n‑3) – 15*a(n‑4) + 6*a(n‑5) – a(n‑6)
 *
 *  The function memoises computed values in a static dynamic array so that
 *  calls in arbitrary order remain fast.  It uses only the allowed arithmetic
 *  and pointer operations and the standard library realloc.  The code is
 *  valid for C11 and C++17 and works for all supported seq_t and idx_t
 *  combinations.  It does not use any of the helper functions supplied by
 *  the test harness, except the standard malloc/realloc for memory
 *  management. */

#include <stdlib.h>   /* malloc, realloc, abort */

/* static cache for memoisation */
static seq_t *cache      = NULL;  /* pointer to the array of computed terms   */
static size_t cache_len  = 0;     /* allocated capacity (number of elements)  */
static size_t init_len   = 0;     /* number of elements actually initialised */

/* Ensure that the cache can hold elements up to index n (inclusive) */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;          /* we need index n */
    if (cache_len >= need)                /* already enough */
        return;

    size_t new_cap = cache_len ? cache_len : 16;
    while (new_cap < need)
        new_cap <<= 1;                    /* double until big enough */

    /* realloc the array; this works for any type of seq_t */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0)              /* allocation failure */
        abort();

    cache     = tmp;
    cache_len = new_cap;

    /* initialise the first six values only once */
    if (init_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)34;
        cache[3] = (seq_t)153;
        cache[4] = (seq_t)686;
        cache[5] = (seq_t)3088;
        init_len = 6;
    }
}

/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Make sure the cache can hold up to index n */
    ensure_capacity(n);

    size_t idx = (size_t)n;

    /* If n is one of the initial terms, return it immediately */
    if (idx < init_len)
        return cache[idx];

    /* Compute missing terms up to n */
    for (size_t i = init_len; i <= idx; ++i) {
        seq_t val = 7 * cache[i - 1]
                  - 15 * cache[i - 2]
                  + 20 * cache[i - 3]
                  - 15 * cache[i - 4]
                  + 6  * cache[i - 5]
                  -  cache[i - 6];
        cache[i] = val;
    }

    init_len = idx + 1;   /* all terms up to idx are now initialised */
    return cache[idx];
}
