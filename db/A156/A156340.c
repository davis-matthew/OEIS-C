/*
 * A156340  G.f.: A(x) = exp( Sum_{n>=1} 2^(n^2-n+1) * x^n/n ), a power series
 *          in x with integer coefficients.
 * https://oeis.org/A156340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0 a(n) = (1/n)*∑_{k=1..n} 2^(k^2-k+1) * a(n-k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* fast binary exponentiation of 2 for an idx_t exponent */
static seq_t pow2_fast(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    while (exp) {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* cache for computed terms */
static seq_t *cache = (seq_t *)NULL;
static idx_t  cache_len   = 0;  /* allocated size */
static idx_t  computed_up_to = 0;  /* highest computed index */

/* memoised recurrence implementation */
seq_t a(idx_t n)
{
    /* initialise cache if necessary */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        computed_up_to = 0;
    }

    /* enlarge cache to accommodate n */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        for (idx_t i = cache_len; i < new_len; ++i)
            cache[i] = (seq_t)0;
        cache_len = new_len;
    }

    /* compute all missing terms up to n */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            /* exponent k^2 - k + 1 */
            idx_t e = (k * k) - k + (idx_t)1;
            seq_t term = pow2_fast(e) * cache[i - k];
            sum += term;
        }
        cache[i] = sum / (seq_t)i;   /* division is exact */
    }
    computed_up_to = n;
    return cache[n];
}
