/*
 * A074361  Coefficient of q^1 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(3,1).
 * https://oeis.org/A074361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 6*a(n-3) - a(n-4) (n>=5) Initial
 *            values: a(0)=0, a(1)=0, a(2)=0, a(3)=3, a(4)=19
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic array of computed a(n) values   */
static idx_t  cache_size = 0;    /* number of valid elements in cache    */
static size_t cache_cap   = 0;    /* allocated capacity (number of elements) */

/* Ensure the cache can hold values up to `n`.  Grow with factor 2.  */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;          /* already computed */
    /* Need capacity for n+1 elements (indices 0..n) */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = needed < 8 ? 8 : needed * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
    /* Initialize new entries (will fill later) */
    for (idx_t i = cache_size; i <= n; ++i) {
        /* For indices < 5, set the known base values */
        if (i == 0) cache[i] = (seq_t)0;
        else if (i == 1) cache[i] = (seq_t)0;
        else if (i == 2) cache[i] = (seq_t)0;
        else if (i == 3) cache[i] = (seq_t)3;
        else if (i == 4) cache[i] = (seq_t)19;
        else cache[i] = (seq_t)0;      /* placeholder, will compute later */
    }
    /* Compute any new terms that are ≥5 using the recurrence */
    for (idx_t k = cache_size; k <= n; ++k) {
        if (k < 5) continue;          /* already set above */
        /* a(k) = 6*a(k-1) - 7*a(k-2) - 6*a(k-3) - a(k-4) */
        seq_t t1 = 6 * cache[k-1];
        seq_t t2 = 7 * cache[k-2];
        seq_t t3 = 6 * cache[k-3];
        seq_t t4 = cache[k-4];
        cache[k] = t1 - t2 - t3 - t4;
    }
    cache_size = n + 1;
}

/* Public entry point */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
