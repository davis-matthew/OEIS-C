/*
 * A052575  Expansion of e.g.f. (1-x)/(1-2*x-2*x^2+2*x^3).
 * https://oeis.org/A052575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   (12+2*n^3+12*n^2+22*n)*a(n) + (-2*n^2-10*n-12)*a(n+1) +
 *            (-2*n-6)*a(n+2) + a(n+3) = 0, a(0)=1, a(1)=1, a(2)=8
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Static cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_computed = 2;    /* last term index that has been filled */

/* Ensure cache is large enough and initialise the first three terms on first use */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initial capacity */
        cache_cap = (n > 10) ? (n + 1) : 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) abort();
        /* initialise base terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)8;
        max_computed = 2;
    }
    if (cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
    if (max_computed < n) {
        /* Compute successive terms using the recurrence */
        for (idx_t next = max_computed + 1; next <= n; ++next) {
            idx_t k = next - 3;                    /* index used in the recurrence */
            seq_t ak  = cache[k];
            seq_t ak1 = cache[k + 1];
            seq_t ak2 = cache[k + 2];

            seq_t kt = (seq_t)k;                   /* cast index to seq_t */

            /* Three coefficients as seq_t */
            seq_t term1 = (seq_t)12
                        + (seq_t)2 * kt * kt * kt
                        + (seq_t)12 * kt * kt
                        + (seq_t)22 * kt;

            seq_t term2 = (seq_t)2 * kt * kt
                        + (seq_t)10 * kt
                        + (seq_t)12;

            seq_t term3 = (seq_t)2 * kt
                        + (seq_t)6;

            /* a(next) = term2*ak1 + term3*ak2 - term1*ak */
            seq_t sum   = term2 * ak1 + term3 * ak2;
            cache[next] = sum - term1 * ak;
        }
        max_computed = n;
    }
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;
    ensure_cache(n);
    return cache[n];
}
