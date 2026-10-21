/*
 * A085110  a(1)=1, then add 1 multiply by 2 to get a(2), subtract 1 and
 *          multiply by 3 to get a(3), add 1 and multiply by 4 to get a(4) and
 *          so on.
 * https://oeis.org/A085110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) + (-1)^n * n, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises computed values so that repeated calls
   (in any order) are efficient.  It grows a dynamic array of seq_t to
   store the sequence as far as it has been computed.  */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* static cache of previously computed terms; cache[i] holds a(i+1) */
static seq_t *cache = NULL;     /* pointer to array of seq_t   */
static idx_t  cache_cap = 0;    /* allocated capacity          */
static idx_t  cache_len = 0;    /* number of terms currently stored */

/* helper to grow the cache to at least size 'need' */
static void
ensure_cache_capacity(idx_t need)
{
    if (cache_cap >= need)
        return;
    idx_t new_cap = (cache_cap == 0) ? 16 : cache_cap;
    while (new_cap < need)
        new_cap <<= 1;                       /* double until large enough */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                              /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* main function that returns the n-th term of the sequence */
seq_t
a(idx_t n)
{
    if (n == 0)        /* not used by the OEIS format but safe */
        return (seq_t)0;

    /* initialise cache if this is the first call */
    if (cache_len == 0) {
        ensure_cache_capacity(16);
        cache[0] = (seq_t)1;   /* a(1) = 1 */
        cache_len = 1;
    }

    /* if already computed, return the stored value */
    if (n <= cache_len)
        return cache[n - 1];

    /* start from the last known value */
    seq_t res = cache[cache_len - 1];
    idx_t i;
    for (i = cache_len + 1; i <= n; ++i) {
        seq_t i_seq = (seq_t)i;
        if ((i & 1) == 0)           /* i even: add i */
            res = res * i_seq + i_seq;
        else                         /* i odd : subtract i */
            res = res * i_seq - i_seq;

        /* store the newly computed term */
        ensure_cache_capacity(i);
        cache[i - 1] = res;
    }

    /* update cache length and return result */
    cache_len = n;
    return res;
}
