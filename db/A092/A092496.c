/*
 * A092496  Molien series for 16-dimensional group of structure S_3 and order
 *          6, corresponding to complete weight enumerators of Hermitian
 *          self-dual GF(4)-linear codes over GF(16).
 * https://oeis.org/A092496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in u1/u2 where u1 = f +
 *            x^16*subs(x=1/x,f) with f = 1 + 10*x^2 + 40*x^3 + 90*x^4 +
 *            180*x^5 + 340*x^6 + 420*x^7 + 215*x^8, u2 =
 *            (1-x)^5*(1-x^2)^6*(1-x^3)^5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 22, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"

static int d_coef[33];      /* denominator coefficients, d_coef[0]==1 */
static int16_t n_coef[17];  /* numerator coefficients, n_coef[0..16] */

/* build denominator and numerator coefficient tables once */
static void init_coeffs(void)
{
    /* (1-x)^5 */
    const int f1[6] = {  1, -5,  10, -10,   5,  -1 };

    /* (1-x^2)^6  -> indices 0..12 */
    const int g[13] = { 1, 0, -6, 0, 15, 0, -20, 0, 15, 0, -6, 0, 1 };

    /* (1-x^3)^5  -> indices 0..15 */
    const int h[16] = { 1, 0, 0, -5, 0, 0, 10, 0, 0, -10, 0, 0, 5, 0, 0, -1 };

    /* first convolution f1 * g  (degree 17) */
    int tmp[33] = {0};
    for (int i = 0; i <= 5; ++i)
        for (int j = 0; j <= 12; ++j)
            tmp[i + j] += f1[i] * g[j];

    /* second convolution tmp * h  (degree 32) */
    for (int i = 0; i <= 32; ++i) d_coef[i] = 0;
    for (int i = 0; i <= 17; ++i)
        for (int j = 0; j <= 15; ++j)
            d_coef[i + j] += tmp[i] * h[j];

    /* numerator coefficients from f + x^16*reverse(f) */
    for (int i = 0; i <= 16; ++i) n_coef[i] = 0;
    n_coef[0] =  1;
    n_coef[2] = 10;
    n_coef[3] = 40;
    n_coef[4] = 90;
    n_coef[5] = 180;
    n_coef[6] = 340;
    n_coef[7] = 420;
    n_coef[8] = 215 + 215;   /* 430 */
    n_coef[9] = 420;
    n_coef[10] = 340;
    n_coef[11] = 180;
    n_coef[12] = 90;
    n_coef[13] = 40;
    n_coef[14] = 10;
    n_coef[16] = 1;
}

/* memoised cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored terms */
static size_t cache_cap = 0;   /* current capacity */

/* enlarge cache to hold indices 0..n */
static void ensure_cache(size_t n)
{
    if (n < cache_len) return;

    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= n) new_cap *= 2;

    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;

    /* initialise new entries to zero (this is safe for built‑in types) */
    for (size_t i = cache_len; i <= n; ++i) cache[i] = 0;

    cache_len = n + 1;
}

/* compute terms up to index n (inclusive) */
static void compute_up_to(size_t n)
{
    static int initialized = 0;
    if (!initialized) { init_coeffs(); initialized = 1; }

    size_t old_len = cache_len;
    ensure_cache(n);

    for (size_t idx = old_len; idx <= n; ++idx)
    {
        seq_t pos = 0, neg = 0;
        size_t maxk = (idx < 32) ? idx : 32;          /* avoid underflow */

        for (size_t k = 1; k <= maxk; ++k)
        {
            int dk = d_coef[k];
            if (dk > 0)          pos += (seq_t)dk * cache[idx - k];
            else if (dk < 0)     neg += (seq_t)(-dk) * cache[idx - k];
        }

        seq_t base = 0;
        if (idx <= 16) base = (seq_t)n_coef[(int)idx];

        /* a[idx] = base + neg - pos */
        seq_t temp = base + neg;
        cache[idx] = (pos > temp) ? 0 : temp - pos;   /* should not happen */
    }
}

/* public interface required by the judge */
seq_t a(idx_t n)
{
    size_t sn = (size_t)n;           /* idx_t is unsigned, safe cast */
    compute_up_to(sn);
    return cache[sn];
}
