/*
 * A177126  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=10, k=1 and l=1.
 * https://oeis.org/A177126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=10; for n>=1: a(n+1)=Σ_{p=0..n} a(p)*a(n-p) + (n+2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed terms */
static seq_t *cache      = NULL;   /* array holding a(0..size-1) */
static idx_t cache_size  = 0;      /* number of stored elements   */
static size_t cache_cap  = 0;      /* capacity of allocated array */

/* Ensure that values up to index n are computed and stored in cache.  */
static void _ensure_up_to(idx_t n)
{
    /* Nothing to do if everything up to n is already cached. */
    if (cache && cache_size > n)
        return;

    /* ---------  allocate or grow the array  --------- */
    size_t new_cap = cache_cap ? cache_cap : 16;
    while ((idx_t)new_cap <= n)
        new_cap <<= 1;

    if (cache == NULL) {
        cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        cache_cap = new_cap;
        cache_size = 0;
    } else if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /*  Initialise newly allocated slots to zero before use.  */
    for (idx_t i = cache_size; i <= n; ++i)
        cache[i] = 0;

    /*  Set the base values – always overwrite to keep them correct.  */
    cache[0] = 1;
    if (n >= 1)
        cache[1] = 10;

    /*  Compute new values from the current cache_size up to n.  */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0 || i == 1) /* already known */
            continue;

        idx_t nn = i - 1;   /* n in the recurrence */
        seq_t sum = 0;
        for (idx_t p = 0; p <= nn; ++p)
            sum += cache[p] * cache[nn - p];

        cache[i] = sum + (seq_t)nn + 2;   /* a(i) = sum + (i-1)+2 = sum + i+1 */
    }

    /*  Update the size of the known prefix.  */
    cache_size = n + 1;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    _ensure_up_to(n);
    return cache[n];
}
