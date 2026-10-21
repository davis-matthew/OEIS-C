/*
 * A096484  Integer part of the square root of (2n-1)-th decimal repunit.
 * https://oeis.org/A096484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = floor(sqrt((10^(2*n-1)-1)/9))
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* sequence function ------------------------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;    /* largest index computed so far          */
static seq_t curr_a = 0;       /* a(n) for n = cache_max                */
static seq_t curr_d = 0;       /* error term for current a(n)           */

/* ensure the cache can hold indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* just to avoid undefined usage */

    /* return cached value if already computed */
    if (n <= cache_max)
        return cache[n];

    /* initialise first term if needed */
    if (cache_max == 0) {
        ensure_cache(1);
        cache[1] = (seq_t)1;
        cache_max = 1;
        curr_a = (seq_t)1;
        curr_d = (seq_t)0;
    }

    /* compute subsequent terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t a_prev = curr_a;
        seq_t d_prev = curr_d;

        seq_t X = a_prev * (seq_t)10;               /* 10 * a_prev         */
        seq_t Y = d_prev * (seq_t)100 + (seq_t)11;  /* 100 * d_prev + 11   */

        /* find largest k such that 2*X*k + k^2 <= Y   (k <= 10) */
        unsigned int k_val = 0;
        for (int kk = 10; kk >= 0; --kk) {
            seq_t t = X * (seq_t)(2 * kk) + (seq_t)(kk * kk);
            if (t <= Y) {
                k_val = (unsigned int)kk;
                break;
            }
        }
        seq_t k = (seq_t)k_val;

        /* update a and d for next term */
        curr_a = X + k;
        seq_t temp = X * (seq_t)(2 * k_val) + (seq_t)(k_val * k_val);
        curr_d = Y - temp;

        ensure_cache(i);
        cache[i] = curr_a;
    }

    cache_max = n;
    return cache[n];
}
