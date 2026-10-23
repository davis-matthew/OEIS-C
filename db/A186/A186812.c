/*
 * A186812  Expansion of 1/(1-x^6-3*x^5-4*x^4-3*x^3-2*x^2-x).
 * https://oeis.org/A186812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the recurrence * a(0) = 1, a(1) = 1, a(2) = 3,
 *            a(3) = 8, a(4) = 21, a(5) = 53, * and for n>=6: * a(n) = a(n-1)
 *            + 2*a(n-2) + 3*a(n-3) + 4*a(n-4) + 3*a(n-5) + a(n-6)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of valid terms stored */
static size_t cache_cap = 0;   /* allocated capacity */

static void ensure_alloc(size_t cap)
{
    if (cache_cap >= cap)
        return;
#ifdef __cplusplus
    seq_t *new_arr = new seq_t[cap];
#else
    seq_t *new_arr = (seq_t *)malloc(sizeof(seq_t) * cap);
#endif
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_arr[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }
    cache = new_arr;
    cache_cap = cap;
}

static void init_cache(void)
{
    if (cache_len > 0)
        return;
    ensure_alloc(6);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)8;
    cache[4] = (seq_t)21;
    cache[5] = (seq_t)53;
    cache_len = 6;
}

static void extend_cache(idx_t n)
{
    if ((size_t)n < cache_len)
        return;
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        ensure_alloc(new_cap);
    }
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t val = cache[i-1];
        val += (seq_t)2 * cache[i-2];
        val += (seq_t)3 * cache[i-3];
        val += (seq_t)4 * cache[i-4];
        val += (seq_t)3 * cache[i-5];
        val += cache[i-6];
        cache[i] = val;
    }
    cache_len = needed;
}

seq_t a(idx_t n)
{
    if (n > 9223372036854775807ULL)   /* protect against absurdly large n */
        return (seq_t)0;
    init_cache();
    extend_cache(n);
    return cache[(size_t)n];
}
