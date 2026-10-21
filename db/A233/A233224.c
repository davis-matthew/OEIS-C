/*
 * A233224  Number of n X 2 0..5 arrays with no element x(i,j) adjacent to
 *          value 5-x(i,j) horizontally, diagonally, antidiagonally or
 *          vertically, top left element zero, and 1 appearing before 2 3 and
 *          4, and 2 appearing before 3 in row major order.
 * https://oeis.org/A233224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 21*a(n-1) - 79*a(n-2) - 41*a(n-3) + 72*a(n-4) + 28*a(n-5)
   Initial values: a(1)=2, a(2)=14, a(3)=174, a(4)=2541, a(5)=39203 */

#include <stdlib.h>
#include <string.h>

/* Static cache: 1‑based indices.  cache[0] is unused. */
static seq_t *cache = NULL;      /* array of cached terms */
static size_t cache_len = 0;     /* highest index that has been computed */
static size_t cache_cap = 0;     /* current allocation size (indices 0..cache_cap-1) */

/* Ensure that the cache can hold indices up to 'needed' (inclusive). */
static void ensure_cap(size_t needed)
{
    if (needed < cache_cap) return;          /* already large enough */

    size_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= needed) new_cap <<= 1;  /* double until large enough */

#ifdef __cplusplus
    seq_t *new_arr = new seq_t[new_cap];
#else
    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif

    if (cache) {
#ifdef __cplusplus
        for (size_t i = 0; i <= cache_len; ++i) new_arr[i] = cache[i];
        delete[] cache;
#else
        memcpy(new_arr, cache, (cache_len + 1) * sizeof(seq_t));
        free(cache);
#endif
    } else {
#ifdef __cplusplus
        /* default‑constructed elements are fine */
#else
        /* zero the memory; not strictly needed but keeps debugging easier */
        memset(new_arr, 0, new_cap * sizeof(seq_t));
#endif
    }

    cache = new_arr;
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence defined from n=1 onward */

    size_t target = (size_t)n;     /* index we need to compute up to */

    /* Grow cache if necessary.  We need indices 0..target inclusive. */
    ensure_cap(target);

    /* Compute any missing terms up to 'target'. */
    for (size_t k = cache_len + 1; k <= target; ++k) {
        if (k == 1)          cache[1] = (seq_t)2;
        else if (k == 2)     cache[2] = (seq_t)14;
        else if (k == 3)     cache[3] = (seq_t)174;
        else if (k == 4)     cache[4] = (seq_t)2541;
        else if (k == 5)     cache[5] = (seq_t)39203;
        else {               /* recurrence for k ≥ 6 */
            seq_t pos = cache[k-1] * 21
                       + cache[k-4] * 72
                       + cache[k-5] * 28;

            seq_t neg = cache[k-2] * 79
                       + cache[k-3] * 41;

            cache[k] = pos - neg;
        }
    }

    cache_len = target;
    return cache[n];
}
