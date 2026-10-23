/*
 * A165125  Number of n-digit fixed points under the base-9 Kaprekar map
 *          A165110.
 * https://oeis.org/A165125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-2) - a(n-3) + a(n-5) - a(n-6) + a(n-8) + a(n-15) -
 *            a(n-17) + a(n-18) - a(n-20) + a(n-21) - a(n-23) for n > 24;
 *            initial values (n = 1 … 24):
 *            1,0,0,0,1,0,0,1,1,1,1,0,1,2,1,1,2,2,3,2,2,1,4,2 The function
 *            memoises results in a static array. The recurrence is evaluated
 *            with signed `long lon...
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] == a(i+1)                            */
static size_t cache_cap = 0;     /* allocated capacity                            */
static idx_t  cache_size = 0;    /* number of computed elements                    */

/* Ensure that the cache can hold at least n values (indices 0 … n-1). */
static void ensure_cache_capacity(idx_t n)
{
    if (cache_cap >= (size_t)n)
        return;

    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 32;
    if (new_cap < (size_t)n)
        new_cap = n;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                 /* terminate on allocation failure */

    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* sequence is defined for n ≥ 1 (offset 1)        */
        return 0;

    /* compute values up to n, if not already cached               */
    if (cache_size < n) {
        ensure_cache_capacity(n);
        for (idx_t idx = cache_size; idx < n; ++idx) {
            idx_t k = idx + 1;          /* current n for the recurrence       */
            seq_t val;

            if (k <= 24) {              /* use known initial values */
                static const seq_t init[24] = {
                    1,0,0,0,1,0,0,1,1,1,1,0,1,2,1,1,2,2,3,2,2,1,4,2
                };
                val = init[k-1];
            } else {                    /* recurrence for k > 24            */
                long long res = 0;
                res += (long long)cache[k-3];   /* a(k-2)  */
                res -= (long long)cache[k-4];   /* a(k-3)  */
                res += (long long)cache[k-6];   /* a(k-5)  */
                res -= (long long)cache[k-7];   /* a(k-6)  */
                res += (long long)cache[k-9];   /* a(k-8)  */
                res += (long long)cache[k-16];  /* a(k-15) */
                res -= (long long)cache[k-18];  /* a(k-17) */
                res += (long long)cache[k-19];  /* a(k-18) (sign corrected) */
                res -= (long long)cache[k-21];  /* a(k-20) */
                res += (long long)cache[k-22];  /* a(k-21) */
                res -= (long long)cache[k-24];  /* a(k-23) */
                val = (seq_t)res;
            }

            cache[idx] = val;
        }
        cache_size = n;
    }

    return cache[n-1];
}
