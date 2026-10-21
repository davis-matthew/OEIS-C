/*
 * A052150  Partial sums of A000340, second partial sums of A003462.
 * https://oeis.org/A052150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + binom(n+2,2) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for computed terms.  cache[i] holds a(i). */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* Allocated elements */
static size_t computed_upto = 0;   /* Highest index computed */
static int initialized = 0;        /* a(0) stored? */

/* Ensure that cache can store indices up to n (inclusive). */
static void ensure_cache(size_t n)
{
    if (cache_size > n)
        return;                    /* already large enough */

    size_t new_size = cache_size ? cache_size : 1;
    while (new_size <= n)
        new_size <<= 1;           /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                  /* allocation failed */

    cache = tmp;
    cache_size = new_size;
}

/* Compute the nth term of OEIS A052150. */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;            /* for internal indexing */

    /* Make sure the cache can hold this index. */
    ensure_cache(idx);

    /* One‑time initialization of a(0). */
    if (!initialized) {
        cache[0] = (seq_t)1;            /* a(0) = 1 */
        computed_upto = 0;
        initialized    = 1;
    }

    /* If we haven't reached n yet, compute missing terms. */
    if (computed_upto < idx) {
        for (size_t i = computed_upto + 1; i <= idx; ++i) {
            /* binom(i+2,2) = (i+2)*(i+1)/2 */
            seq_t binom = ((seq_t)(i + 2) * (seq_t)(i + 1)) / 2;
            seq_t val   = (seq_t)3 * cache[i - 1] + binom;
            cache[i]    = val;
        }
        computed_upto = idx;
    }

    return cache[idx];
}
