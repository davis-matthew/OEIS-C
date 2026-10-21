/*
 * A211567  Number of -2..2 arrays x(i) of n+1 elements i=1..n+1 with
 *          set{t,u,v in 0,1}((x[i+t]+x[j+u]+x[k+v])*(-1)^(t+u+v)) having two
 *          or four distinct values for every i,j,k<=n.
 * https://oeis.org/A211567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n‑1)+3*a(n‑2)-15*a(n‑3)+4*a(n‑4)+18*a(n‑5)-12*a(n‑6)
   Initial values: a(1)=24, a(2)=40, a(3)=68, a(4)=122, a(5)=220, a(6)=410 */
#include <stdlib.h>

/* Cache for computed terms */
static seq_t *cache = NULL;        /* allocated memory for the cache */
static size_t cache_len = 0;       /* number of terms currently stored */
static size_t cache_cap = 0;       /* current capacity of the cache */

/* Seed values for the recurrence */
static const seq_t init[6] = {
    (seq_t)24, (seq_t)40, (seq_t)68,
    (seq_t)122, (seq_t)220, (seq_t)410
};

/* Grow the cache so that it can contain at least 'needed' elements.
   The returned pointer is assigned to 'cache'. */
static void ensure_cache_size(size_t needed)
{
    if (needed <= cache_cap)
        return;
    seq_t *newptr = (seq_t *)realloc(cache, sizeof(seq_t) * needed);
    if (newptr == NULL)
        abort();                    /* out of memory */
    cache = newptr;
    cache_cap = needed;
}

/* Compute a(n) for n >= 1.  a(0) is undefined; for safety we return 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for n == 0 */

    /* Convert to zero‑based index into the cache array */
    size_t idx = (size_t)(n - 1);

    /* If we already have the term, return it immediately */
    if (idx < cache_len)
        return cache[idx];

    /* Ensure the cache can hold all terms up to idx, then generate them */
    if (idx >= cache_len) {
        ensure_cache_size(idx + 1);
        for (size_t k = cache_len; k <= idx; ++k) {
            seq_t val;
            if (k < 6)            /* seed values */
                val = init[k];
            else                  /* recurrence */
                val = 3 * cache[k - 1]
                      + 3 * cache[k - 2]
                      - 15 * cache[k - 3]
                      + 4 * cache[k - 4]
                      + 18 * cache[k - 5]
                      - 12 * cache[k - 6];
            cache[k] = val;
        }
        cache_len = idx + 1;
    }
    return cache[idx];
}
