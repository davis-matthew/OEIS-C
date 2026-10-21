/*
 * A050004  a(n) = floor(a(n-1)/2) if this is not among 0, a(1), ..., a(n-2);
 *          otherwise a(n) = 5*a(n-1).
 * https://oeis.org/A050004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(a(n-1)/2) if not among {0 , a(1),...,a(n-2)};
 *            otherwise a(n) = 5*a(n-1). Implemented with memoisation
 * Range:     u8 n <= 17, u16 n <= 417, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;      /* array of previously computed terms */
static size_t cache_len = 0;     /* number of terms stored in cache */
static size_t cache_cap = 0;     /* allocated capacity of cache */

/* Grow the cache array to at least new_cap elements. */
static void grow_cache(size_t new_cap) {
    if (new_cap <= cache_cap)
        return;
    /* choose a doubling strategy to keep amortised linear time */
    size_t cap = cache_cap > 0 ? cache_cap : 4;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *ptr = (seq_t*)realloc(cache, cap * sizeof(seq_t));
    if (!ptr) abort();           /* out of memory */
    cache = ptr;
    cache_cap = cap;
}

/* Return a(n) for n >= 1.  The cache keeps all terms computed so far. */
seq_t a(idx_t n) {
    if (n == 0) {
        /* Undefined for n=0; return 0 or abort. Here we return 0. */
        return (seq_t)0;
    }
    size_t up_to = (size_t)n;          /* target number of terms to have */
    if (up_to > cache_cap)
        grow_cache(up_to);
    /* Compute any missing terms up to n */
    while (cache_len < up_to) {
        if (cache_len == 0) {          /* a(1) */
            cache[0] = (seq_t)1;
            cache_len = 1;
            continue;
        }
        seq_t prev = cache[cache_len - 1];
        seq_t candidate = prev / (seq_t)2;   /* floor(prev/2) */
        bool use_mul5 = false;

        if (candidate == (seq_t)0) {
            use_mul5 = true;          /* the set explicitly contains 0 */
        } else {
            /* check earlier terms a(1) ... a(n-2) */
            /* cache_len-1 is index of prev (a(n-1)), so search up to cache_len-2 */
            for (size_t j = 0; j < cache_len - 1; ++j) {
                if (cache[j] == candidate) {
                    use_mul5 = true;
                    break;
                }
            }
        }

        seq_t val = use_mul5 ? (seq_t)5 * prev : candidate;
        cache[cache_len] = val;
        ++cache_len;
    }
    return cache[up_to - 1];
}
