/*
 * A081043  8th binomial transform of (1,7,0,0,0,0,0,...).
 * https://oeis.org/A081043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16*a(n-1) - 64*a(n-2), a(0)=1, a(1)=15
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises computed terms in a static array so that
   any order of calls receives the correct value.  The recurrence is
   evaluated iteratively; the computation is O(n) in the largest n
   requested, with negligible overhead for repeated calls.  All arithmetic
   uses the seq_t type provided by the build. */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed terms                */
static idx_t cache_len = 0;      /* allocated length (power of two)        */
static idx_t cache_upto = 0;     /* highest index that is computed.       */

/* Ensure that cache contains a(0) ... a(n).  The array capacity is
   increased as a power of two to minimise reallocations. */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* Initialise capacity to maximum of 2 or (n+1).  We store at
           least the first two terms. */
        idx_t init_cap = ((n + 1) >= 2) ? (n + 1) : 2;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!cache) return;        /* allocation failure; caller will fail */
        cache_len = init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)15;
        /* Compute any additional terms up to init_cap-1 (if needed). */
        for (idx_t i = 2; i < cache_len; ++i) {
            cache[i] = 16 * cache[i - 1] - 64 * cache[i - 2];
        }
        cache_upto = cache_len - 1;
        return;
    }

    /* Already have some data; extend if necessary. */
    if (n <= cache_upto) return;

    /* Determine new capacity: a power of two greater than n. */
    idx_t new_cap = cache_len;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;      /* allocation failure; caller will fail */
    cache = tmp;
    cache_len = new_cap;

    /* Compute terms from cache_upto+1 up to n (and beyond a little,
       but never beyond the new capacity). */
    for (idx_t i = cache_upto + 1; i <= n; ++i) {
        cache[i] = 16 * cache[i - 1] - 64 * cache[i - 2];
    }
    cache_upto = n;
}

seq_t
a(idx_t n)
{
    ensure_cache(n);
    if (n <= cache_upto) return cache[n];
    /* Should not happen – if ensure_cache failed, return 0. */
    return (seq_t)0;
}
