/*
 * A205219  Number of (n+1)X2 0..1 arrays with the number of equal 2X2
 *          subblock diagonal pairs and equal antidiagonal pairs differing
 *          from each horizontal or vertical neighbor, and new values 0..1
 *          introduced in row major order.
 * https://oeis.org/A205219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=8, a(2)=20, a(n)=a(n-1)+4*a(n-2) for n≥3
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/*  Memoised recurrence solver for A205219                       */
/* ------------------------------------------------------------ */

#include <stddef.h>   /* size_t, NULL */
#include <stdlib.h>   /* malloc, realloc */

/* ------------------------------------------------------------ */
/*  Static cache:  cache_len = number of terms stored (≥0)     */
/*  cache_cap = allocated capacity (≥cache_len)                */
/* ------------------------------------------------------------ */

static seq_t *cache = NULL;     /* array holding computed terms    */
static idx_t     cache_len  = 0;/* number of initialized terms     */
static idx_t     cache_cap  = 0;/* allocated size of the array     */

/* ------------------------------------------------------------ */
/*  Grow the cache to at least 'new_cap' elements (capacity)    */
/* ------------------------------------------------------------ */
static void grow_cache(idx_t new_cap)
{
    /* new_cap is always > cache_cap */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp != NULL) {
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------ */
/*  a(n) – public entry point                                  */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)             /* offset 1 – undefined for n=0            */
        return (seq_t)0;

    /* lazy initialisation of cache for the first two terms   */
    if (cache_len == 0) {
        /* allocate space for the first two elements          */
        idx_t init_cap = 2;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL)        /* allocation failure – return 0       */
            return (seq_t)0;

        cache[0] = (seq_t)8;      /* a(1) */
        cache[1] = (seq_t)20;     /* a(2) */
        cache_len = 2;
        cache_cap = init_cap;
    }

    /* If n is already computed, return it immediately        */
    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure the array has enough capacity for the new term   */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : n;
        if (new_cap < n)
            new_cap = n;
        grow_cache(new_cap);
    }

    /* Compute missing terms from cache_len up to n-1 (inclusive) */
    for (idx_t i = cache_len; i < n; ++i) {
        /* i corresponds to term a(i+1)                     */
        /* recurrence: a(i+1) = a(i) + 4*a(i-1)              */
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = prev1 + (seq_t)4 * prev2;
    }

    /* Update the number of stored terms */
    cache_len = n;

    return cache[n - 1];
}
