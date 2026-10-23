/*
 * A017845  Expansion of 1/(1-x^5-x^6-x^7-x^8-x^9-x^10-x^11-x^12-x^13-x^14).
 * https://oeis.org/A017845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5)+a(n-6)+a(n-7)+a(n-8)+a(n-9)+a(n-10)+ *
 *            a(n-11)+a(n-12)+a(n-13)+a(n-14) (n>13), * with a(0)=1,
 *            a(1)=a(2)=a(3)=a(4)=0. * * The function computes the sequence
 *            lazily and memoises all * previously requested values. It works
 *            with both simple integral * types and with classes that
 *            represe...
 * Range:     u8 n <= 27, u16 n <= 48, u32 n <= 88, u64 n <= 169,
 *            u128 n <= 332, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifndef __cplusplus
/* C mode: seq_t is an integral type, realloc works */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;      /* current allocated size (=max + 1)   */
static idx_t  computed_max = 0;   /* largest index that has been computed */

static void
ensure_cache(idx_t n)
{
    if (cache_len <= n) {                  /* grow the array */
        idx_t new_len = n + 1;
        seq_t *new_cache = realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();           /* allocation failed */
        /* zero the newly allocated part */
        for (idx_t i = cache_len; i < new_len; ++i)
            new_cache[i] = 0;
        cache = new_cache;
        cache_len = new_len;
    }
}
#else
/* C++ mode: seq_t may be a class, use new[] and delete[] */
#include <new>

static seq_t *cache = nullptr;
static idx_t  cache_len = 0;      /* current allocated size (=max + 1)   */
static idx_t  computed_max = 0;   /* largest index that has been computed */

static void
ensure_cache(idx_t n)
{
    if (cache_len <= n) {                  /* grow the array */
        idx_t new_len = n + 1;
        seq_t *new_cache = new seq_t[new_len];
        /* copy existing values */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        /* zero the newly allocated part */
        for (idx_t i = cache_len; i < new_len; ++i)
            new_cache[i] = seq_t(0);
        delete[] cache;
        cache = new_cache;
        cache_len = new_len;
    }
}
#endif

seq_t a(idx_t n)
{
    /* Ensure we have storage for index n */
    ensure_cache(n);

    /* Initialise a(0) if it hasn't been set yet */
    if (cache_len > 0 && cache[0] == (seq_t)0)
        cache[0] = (seq_t)1;

    /* Compute any new entries needed */
    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 5; k <= 14; ++k) {
                if (i >= k)
                    sum += cache[i - k];
            }
            cache[i] = sum;
        }
        computed_max = n;
    }

    return cache[n];
}
