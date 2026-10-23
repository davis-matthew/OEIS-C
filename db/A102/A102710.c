/*
 * A102710  a(1) = 2, a(2) = 3, a(n+2) = a(n)*(a(n)+a(n+1)) - a(n+1).
 * https://oeis.org/A102710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 2, a(2) = 3, a(n+2) = a(n) * (a(n)+a(n+1)) - a(n+1)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* NULL */
#include <stdint.h>
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;        /* cache[i] holds a(i+1)  */
static idx_t  cache_len = 0;       /* number of terms cached  */

/* Return the n‑th term of OEIS sequence A102710. */
seq_t a(idx_t n)
{
    /* Index 0 is undefined; return 0 for safety. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Initialise cache with first two terms on first call. */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) {
            return (seq_t)0;    /* allocation failure – unlikely in tests */
        }
        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)3;   /* a(2) */
    }

    /* If already computed, return cached value. */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Grow the cache to hold n terms. */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!tmp) {
        return (seq_t)0;   /* allocation failure – unlikely in tests */
    }
    cache = tmp;

    /* Compute new terms using the recurrence. */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t a_prev2 = cache[i - 2];   /* a(i-1)   */
        seq_t a_prev1 = cache[i - 1];   /* a(i)     */
        seq_t sum  = a_prev2 + a_prev1;
        seq_t prod = a_prev2 * sum;
        seq_t a_new = prod - a_prev1;
        cache[i] = a_new;
    }

    cache_len = n;
    return cache[n - 1];
}
