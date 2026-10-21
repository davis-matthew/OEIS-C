/*
 * A087192  a(n) = ceiling(a(n-1)*4/3), with a(1) = 1.
 * https://oeis.org/A087192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(a(n-1)*4/3), a(1) = 1
 * Range:     u8 n <= 17, u16 n <= 37, u32 n <= 75, u64 n <= 152,
 *            u128 n <= 306, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is implemented by the identity
   a(n) = a(n-1) + ceil(a(n-1)/3)
   which requires only addition and a division that never overflows
   for any term that still fits in the target integer type. */

#include <stdlib.h>   /* malloc, realloc, abort */

/* Cache of previously computed terms.  a_cache[0] is unused. */
static seq_t *a_cache = NULL;     /* array of computed values */
static size_t cache_len = 0;      /* highest index that has been computed
                                    (a_cache[1] .. a_cache[cache_len-1]) */
static size_t cache_cap = 0;      /* allocated capacity of a_cache */

/* Ensure the cache can hold terms up to index n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* memory exhausted */
        a_cache = tmp;
        cache_cap = new_cap;
    }
}

/* Compute all terms from cache_len up to n (n >= 1). */
static void compute_up_to(idx_t n)
{
    ensure_capacity(n);
    /* start from the first index that has not yet been computed */
    size_t start = cache_len > 1 ? cache_len : 1;

    for (size_t i = start; i <= (size_t)n; ++i) {
        if (i == 1) {
            a_cache[1] = (seq_t)1;
        } else {
            seq_t prev = a_cache[i - 1];
            seq_t next = prev + (prev + 2) / 3;  /* ceil(prev/3) = (prev+2)/3 */
            a_cache[i] = next;
        }
    }
    if (cache_len <= (size_t)n)
        cache_len = (size_t)n + 1;   /* now we have terms up to n */
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n = 0, return 0 */
        return (seq_t)0;

    if (!a_cache) {        /* first call: allocate cache */
        a_cache = (seq_t *)malloc(8 * sizeof(seq_t));
        if (!a_cache) abort();
        cache_cap = 8;
        cache_len = 0;
    }

    compute_up_to(n);
    return a_cache[n];
}
