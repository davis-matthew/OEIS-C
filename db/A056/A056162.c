/*
 * A056162  a(n) = Sum_{k=0..n} (k!)^(n-k).
 * https://oeis.org/A056162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} (k!)^(n-k)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

static seq_t pow_big(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur    = base;
    while (exp) {
        if (exp & (idx_t)1)
            result *= cur;
        exp >>= 1;
        if (exp)
            cur *= cur;
    }
    return result;
}

/* Dynamic arrays for factorials and cached a(n) values */
static seq_t *fac  = NULL;          /* factorial table: fac[i] = i!   */
static idx_t  fac_len = 0;          /* number of factorials stored   */
static idx_t  fac_cap = 0;          /* allocated capacity of fac array */

static seq_t *cache = NULL;         /* a(n) table: cache[i] = a(i)    */
static idx_t  cache_len = 0;        /* number of cached a(n)          */
static idx_t  cache_cap = 0;        /* allocated capacity of cache     */

/* Ensure factorials up to n are computed */
static void ensure_fac(idx_t n)
{
    if (n < fac_len) return;

    /* grow the factorial array if necessary */
    if (fac_cap == 0) {
        fac_cap = 1;
        fac = (seq_t*)malloc(fac_cap * sizeof(seq_t));
    }
    while (fac_cap <= n) {
        size_t new_cap = fac_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        fac = (seq_t*)realloc(fac, new_cap * sizeof(seq_t));
        fac_cap = new_cap;
    }

    /* compute any missing factorials */
    if (fac_len == 0) {          /* initialise fac[0] = 1 */
        fac[0] = (seq_t)1;
        fac_len = 1;
    }
    for (idx_t i = fac_len; i <= n; ++i) {
        fac[i] = fac[i - 1] * (seq_t)i;
    }
    fac_len = n + 1;
}

/* Compute a(k) for all k up to n (including n) */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    /* grow the cache array if necessary */
    if (cache_cap == 0) {
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= n) new_cap = n + 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing a(k) values */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= i; ++k) {
            seq_t term = pow_big(fac[k], i - k);
            sum += term;
        }
        cache[i] = sum;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    /* Ensure required factorials and cached values exist */
    ensure_fac(n);
    ensure_cache(n);
    return cache[n];
}
