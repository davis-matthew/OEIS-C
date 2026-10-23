/*
 * A194006  E.g.f.: exp( exp(3*x) - 2*exp(2*x) + 2*exp(x) - 1 ).
 * https://oeis.org/A194006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1..n} binom(n-1,k-1) * (3^k - 2*2^k + 2) * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system */
seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache       = NULL;
    static size_t cache_cap   = 0;
    static idx_t   cache_len  = 0;

    /* resize cache if necessary */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        cache_len = 1;
    } else if (n >= (idx_t)cache_len) {
        /* grow cache to at least n+1 entries */
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed; caller may handle */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t bin   = (seq_t)1;          /* binom(i-1,0) */
        seq_t pow2  = (seq_t)2;          /* 2^1 */
        seq_t pow3  = (seq_t)3;          /* 3^1 */

        for (idx_t k = 1; k <= i; ++k) {
            /* coefficient c_k = 3^k - 2*2^k + 2 */
            seq_t tmp = pow2;
            tmp += pow2;                 /* tmp = 2*pow2 */
            seq_t c   = pow3;
            c -= tmp;
            c += (seq_t)2;

            /* term = bin * c * a[i-k] */
            seq_t term = bin * c;
            term *= cache[i - k];
            sum += term;

            /* update binom for next k: binom(i-1,k) = binom(i-1,k-1) * (i - k) / k */
            if (k < i) {
                seq_t num = (seq_t)(i - k);
                seq_t den = (seq_t)k;
                bin = (bin * num) / den;
            }

            /* update pow2 and pow3 for next k */
            if (k < i) {
                pow2 *= (seq_t)2;
                pow3 *= (seq_t)3;
            }
        }
        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
