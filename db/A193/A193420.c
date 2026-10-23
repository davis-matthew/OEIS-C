/*
 * A193420  -log( Sum_{n>=0} (-x)^n/n!^3 ) = Sum_{n>=1} a(n)*x^n/n!^3.
 * https://oeis.org/A193420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = -(-1)^n * + (1/n) * Σ_{k=1}^{n-1} (-1)^(n-k-1) *
 *            binomial(n,k)^3 * k * a(k) * Initial value: a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/seq_mul.h"


static seq_t *cache      = NULL;  /* memoized values a(1)…a(m) */
static size_t cache_cap  = 0;     /* allocated capacity of cache */
static idx_t  cache_len  = 0;     /* highest index computed so far */

/* Ensure that cache holds values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* First allocation: size n+1 (indices 0..n). */
        cache_cap = (size_t)n + 1;
        cache = seq_alloc(cache_cap);
        cache[0] = (seq_t)0;          /* unused placeholder */
        cache[1] = (seq_t)1;          /* a(1) = 1 */
        cache_len = 1;
    } else if ((size_t)n + 1 > cache_cap) {
        /* Grow the cache */
        cache = seq_realloc(cache, cache_cap, (size_t)n + 1);
        cache_cap = (size_t)n + 1;
    }

    /* Compute new values from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t pos_sum = (seq_t)0;
        seq_t neg_sum = (seq_t)0;

        /* Compute binomial(i,k) iteratively to avoid factorials. */
        seq_t bin = (seq_t)i;  /* binomial(i,1) */
        for (idx_t k = 1; k < i; ++k) {
            /* term = binomial(i,k)^3 * k * a(k) */
            seq_t bin_sq  = seq_mul(bin, bin);
            seq_t bin_cu  = seq_mul(bin_sq, bin);
            seq_t temp    = seq_mul(bin_cu, (seq_t)k);
            seq_t term    = seq_mul(temp, cache[k]);

            /* sign: (-1)^(i-k-1) => +1 if even, -1 if odd */
            if (((i - k - 1) & 1) == 0) {
                pos_sum += term;
            } else {
                neg_sum += term;
            }

            /* Prepare next binomial value for k+1 */
            if (k + 1 < i) {
                /* bin = bin * (i - k) / (k + 1) */
                seq_t mul_factor = (seq_t)(i - k);
                seq_t div_factor = (seq_t)(k + 1);
                bin = seq_mul(bin, mul_factor);
                bin = bin / div_factor;
            }
        }

        /* sum = pos_sum - neg_sum (safe since pos_sum >= neg_sum) */
        seq_t sum = pos_sum;
        sum -= neg_sum;

        /* Divide by i */
        seq_t sum_div = sum / (seq_t)i;

        /* Final term: +1 for odd i, -1 for even i */
        if ((i & 1) == 0) {
            cache[i] = sum_div - (seq_t)1;
        } else {
            cache[i] = sum_div + (seq_t)1;
        }
    }

    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;     /* unspecified for offset 1 */
    }
    ensure_cache(n);
    return cache[n];
}
