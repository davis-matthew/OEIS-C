/*
 * A120461  Expansion of x*(4-x)/( (2x-1)*(x^2-x-1)).
 * https://oeis.org/A120461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3), a(0)=0, a(1)=4, a(2)=3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  O(n) iterative computation with memoisation. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
/* In C++ we use new[] / delete[] for proper construction/destruction. */
#define SEQ_ALLOC(cap)      new seq_t[(cap)]
#define SEQ_FREE(ptr)      delete [] (ptr)
#define SEQ_MEMCPY(dst, src, n) \
    do { for(size_t i=0; i<(n); ++i) (dst)[i] = (src)[i]; } while(0)
#else
/* In C we use malloc/realloc/free. */
#define SEQ_ALLOC(cap)      ((seq_t *)malloc((cap) * sizeof(seq_t)))
#define SEQ_FREE(ptr)      free((ptr))
#define SEQ_MEMCPY(dst, src, n)  memcpy((dst),(src), (n)*sizeof(seq_t))
#endif

/* Static cache for memoised values. */
static seq_t *seq_cache = NULL;   /* pointer to array of seq_t */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_len = 0;      /* number of values that are already computed */

/* Ensure the cache has at least (needed+1) capacity. */
static void cache_ensure_cap(size_t needed)
{
    if (needed < cache_cap) return;
    size_t new_cap = cache_cap? cache_cap*2 : 4;
    while (new_cap <= needed) new_cap <<= 1;

    seq_t *new_arr = SEQ_ALLOC(new_cap);
    if (seq_cache) {
        SEQ_MEMCPY(new_arr, seq_cache, cache_cap);
        SEQ_FREE(seq_cache);
    }
    seq_cache = new_arr;
    cache_cap = new_cap;
}

/* Compute all terms from cache_len up to and including n. */
static void cache_compute_up_to(idx_t n)
{
    size_t cap_needed = (size_t)n + 1;
    cache_ensure_cap(cap_needed);

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            seq_cache[0] = (seq_t)0;
        } else if (i == 1) {
            seq_cache[1] = (seq_t)4;
        } else if (i == 2) {
            seq_cache[2] = (seq_t)3;
        } else {
            seq_cache[(size_t)i] = seq_cache[(size_t)(i-1)]
                                  + (seq_t)3 * seq_cache[(size_t)(i-2)]
                                  - (seq_t)2 * seq_cache[(size_t)(i-3)];
        }
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    cache_compute_up_to(n);
    return seq_cache[(size_t)n];
}
