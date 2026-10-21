/*
 * A218302  E.g.f. A(x) satisfies A( x/(exp(x)*cosh(x)) ) =
 *          exp(4*x)*cosh(4*x).
 * https://oeis.org/A218302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   pow_uint, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A218302
    Formula: a(n) = 2 * Σ_{k=0..n} binom(n,k) * (k+4)^(n-1)
    a(0) = 1
*/

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/pow_uint.h"    /* provides pow_uint(uint64_t, idx_t) */

static seq_t *cache      = NULL; /* cached values a(0)...a(N)   */
static size_t cache_sz   = 0;    /* number of elements stored    */

static seq_t *pow_arr    = NULL; /* powers (k+4)^(current_exp)  */
static size_t pow_sz     = 0;    /* current allocated size       */

static seq_t *c_arr      = NULL; /* current row of binomial coeff*/
static size_t c_sz       = 0;    /* current size                 */

/* Ensure cache has room for indices up to 'need' inclusive. */
static void ensure_cache(size_t need)
{
    if (cache_sz <= need) {
        cache = seq_realloc(cache, cache_sz, need + 1);
        for (size_t i = cache_sz; i <= need; ++i)
            cache[i] = 0;
        cache_sz = need + 1;
    }
}

/* Ensure pow_arr has room for indices up to 'need' inclusive. */
static void ensure_pow(size_t need)
{
    if (pow_sz <= need) {
        pow_arr = seq_realloc(pow_arr, pow_sz, need + 1);
        for (size_t i = pow_sz; i <= need; ++i)
            pow_arr[i] = 1;          /* (k+4)^0 = 1 for new indices   */
        pow_sz = need + 1;
    }
}

/* Ensure c_arr has room for indices up to 'need' inclusive. */
static void ensure_c(size_t need)
{
    if (c_sz <= need) {
        c_arr = seq_realloc(c_arr, c_sz, need + 1);
        for (size_t i = c_sz; i <= need; ++i)
            c_arr[i] = 0;
        c_sz = need + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        if (cache_sz == 0) {
            cache = seq_alloc(1);
            cache[0] = 1;
            cache_sz = 1;
        }
        return 1;
    }

    /* Extend cached values up to n */
    for (idx_t m = cache_sz; m <= n; ++m) {

        /* ---------- Update powers: p_k = (k+4)^(m-1) ------------- */
        ensure_pow(m);
        if (m > 1) {
            for (idx_t k = 0; k < m; ++k)
                pow_arr[k] = pow_arr[k] * (seq_t)(k + 4);
        }
        /* new index m: exponent n-1 = m-1 */
        pow_arr[m] = pow_uint((uint64_t)(m + 4), (idx_t)(m - 1));

        /* ---------- Update binomial coefficients C[n][k] --------- */
        ensure_c(m);
        if (m == 1) {
            c_arr[0] = 1;
            c_arr[1] = 1;
        } else {
            /* Pascal's rule, iterate backwards to use in‑place */
            c_arr[m] = 1;                     /* C(m,m) = 1 */
            for (idx_t k = m - 1; k > 0; --k)
                c_arr[k] = c_arr[k] + c_arr[k - 1];
            c_arr[0] = 1;                     /* C(m,0) = 1 */
        }

        /* ---------- Compute a(m) = 2 * Σ C(m,k) * p_k ------------- */
        seq_t sum = 0;
        for (idx_t k = 0; k <= m; ++k)
            sum = sum + (c_arr[k] * pow_arr[k]);

        seq_t val = sum * 2;
        ensure_cache(m);
        cache[m] = val;
    }

    return cache[n];
}
