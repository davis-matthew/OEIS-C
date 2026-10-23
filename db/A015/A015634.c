/*
 * A015634  Number of ordered quadruples of integers from [ 1..n ] with no
 *          global factor.
 * https://oeis.org/A015634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n+3,4) - Σ_{j=2..n} a(floor(n/j)) (A015634)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 576, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cached sequence values.  cache[0] is unused. */
static seq_t *cache = NULL;
/* Number of allocated slots (size of cache).   cache_size > 0 means at
   least cache[0..cache_size-1] is valid. */
static size_t cache_size = 0;
/* Highest index already computed in cache (>= 0).*/
static idx_t cache_last = 0;

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if (cache_size > (size_t)n) return;          /* already large enough */
    size_t new_size = (size_t)n + 1;              /* indices 0..n */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();                     /* allocation failure */
    cache = new_cache;
    /* initialise any newly allocated part to zero */
    if (new_size > cache_size) {
        size_t diff = new_size - cache_size;
        memset(cache + cache_size, 0, diff * sizeof(seq_t));
    }
    cache_size = new_size;
}

/* Compute values up to and including n (if not already done). */
static void compute_up_to(idx_t n)
{
    if (n <= cache_last) return;                 /* already computed */

    /* initialise base case a(1) = 1 if needed */
    if (cache_last == 0) {
        ensure_cache_size(1);
        cache[1] = 1;
        cache_last  = 1;
    }

    for (idx_t i = cache_last + 1; i <= n; ++i) {
        /* binomial(i+3,4) = i*(i+1)*(i+2)*(i+3)/24 */
        seq_t binom = ((seq_t)i * (i + 1));
        binom = binom * (i + 2);
        binom = binom * (i + 3);
        binom = binom / ((seq_t)24);

        seq_t sum = 0;
        idx_t j = 2;
        while (j <= i) {
            idx_t q = i / j;                     /* floor(i/j) */
            idx_t next_j = i / q + 1;            /* first j where floor changes */
            sum += cache[q] * (next_j - j);     /* add contribution of all such j */
            j = next_j;
        }
        cache[i] = binom - sum;
    }
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                       /* undefined for n=0, return 0 */
    ensure_cache_size(n);
    compute_up_to(n);
    return cache[n];
}
