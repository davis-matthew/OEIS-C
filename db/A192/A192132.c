/*
 * A192132  G.f. satisfies: A(x) = 1 + x*A(x)^3 + x*(A(x) - 1)^3.
 * https://oeis.org/A192132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Σ_{k=0..⌊(n‑1)/3⌋} binomial(n‑1,k) *
 *            binomial(3*(n‑1)‑3*k+3,(n‑1)‑3*k) / (n‑k) (a(0)=1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence is derived from a(n+1) = Σ_{k=0..⌊n/3⌋}
   binomial(n,k) * binomial(3n‑3k+3,n‑3k) / (n‑k+1). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* external helper provided by the build environment */
extern seq_t binomial(idx_t n, idx_t k);

/* memoisation cache for previously computed terms   */
static seq_t *cache = NULL;       /* cache[0..last_computed] holds a(i) */
static idx_t  cache_size = 0;     /* allocated size of cache */
static idx_t  last_computed = 0;  /* highest index that has been computed */

/* Ensure cache can hold indices 0..n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_size = (n < 16) ? 16 : n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();
        /* initialise to zero */
        for (idx_t i = 0; i < cache_size; ++i) cache[i] = 0;
        cache[0] = 1;              /* a(0) = 1 */
        last_computed = 0;
    } else if (n + 1 > cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size < n + 1) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        /* zero‑initialise the newly allocated part */
        for (idx_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }
}

/* sequence function requested */
seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* a(0) = 1 */

    ensure_cache(n);

    /* compute all missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        idx_t m = i - 1;   /* current n in recurrence */
        seq_t sum = 0;
        idx_t kmax = m / 3;
        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t bi = binomial(m, k);
            seq_t bj = binomial(3 * m - 3 * k + 3,
                                 m - 3 * k);
            seq_t denom = (seq_t)(m - k + 1);
            seq_t term = (bi * bj) / denom;
            sum += term;
        }
        cache[i] = sum;
    }
    last_computed = n;
    return cache[n];
}
