/*
 * A186447  a(n)=a(floor(n/3)+a(n-1)*floor(n/4)) XOR a(floor(n/2)).
 * https://oeis.org/A186447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(floor(n/3)+a(n-1)*floor(n/4)) XOR a(floor(n/2)) Initial
 *            values: a(0)=1, a(1)=1, a(2)=0 This implementation memoises all
 *            computed values in a dynamic array to allow calls in arbitrary
 *            order. The array grows geometrically. It is written to compile
 *            as both C11 and C++17 and works with integ...
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static cache of computed values and a bitmap marking stored entries */
static seq_t  *cache      = NULL;   /* array of seq_t values      */
static unsigned char *known = NULL; /* 0 = unknown, 1 = known    */
static idx_t cache_cap = 0;         /* current allocated length  */

/* enlarge the cache to store at least indices 0..n inclusively */
static void ensure_capacity(idx_t n)
{
    if (n < cache_cap)
        return;

    /* geometric growth: double until we have enough space */
    idx_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= n)
        new_cap <<= 1;

    /* realloc cache for seq_t values */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);   /* out of memory -> abort for safety */

    /* realloc bitmap */
    unsigned char *new_known =
        (unsigned char *)realloc(known, new_cap * sizeof(unsigned char));
    if (!new_known) exit(1);

    /* initialise new entries as unknown */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_known[i] = 0;

    cache      = new_cache;
    known      = new_known;
    cache_cap  = new_cap;
}

/* recursive memoised evaluation of the sequence */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    ensure_capacity(n);

    if (known[n])                  /* already computed? */
        return cache[n];

    /* compute the value recursively */
    seq_t prev   = a(n - 1);                               /* a(n‑1)   */
    idx_t idx1  = (idx_t)(n / 3)
                 + (idx_t)((seq_t)prev * (seq_t)(n / 4));  /* floor(n/3)+a(n‑1)*floor(n/4) */
    idx_t idx2  = n / 2;                                  /* floor(n/2) */

    seq_t term1 = a(idx1);
    seq_t term2 = a(idx2);

    seq_t result = term1 ^ term2;   /* XOR */

    cache[n] = result;
    known[n] = 1;
    return result;
}
