/*
 * A014304  Expansion of e.g.f. 1/sqrt(exp(x)*(2-exp(x))).
 * https://oeis.org/A014304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014304:  Expansion of e.g.f. 1/sqrt(exp(x)*(2-exp(x))).
   Recurrence:
      a(0) = 1
      a(n) = (-1)^n + Σ_{k=0}^{n-1} binomial(n,k) * a(k) * a(n-k-1)   (n ≥ 1)
   Computed with memoisation; binomial coefficients are generated
   multiplicatively to avoid deep recursion.  */
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* stored values  */
static size_t cache_size = 0;        /* allocated size of cache  */
static idx_t  next_computed = 0;     /* smallest index not yet computed */

/* Ensure that cache has room for entries up to n (inclusive). */
static void
ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        size_t new_size = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();   /* allocation failure – terminate */
        cache = tmp;
        /* initialise new cells to zero (handy for arbitrary‑precision types) */
        for (size_t i = cache_size; i < new_size; ++i) {
            cache[i] = 0;
        }
        cache_size = new_size;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Ensure capacity. */
    ensure_cache(n);

    /* Compute missing values up to n. */
    for (idx_t m = next_computed; m <= n; ++m) {
        seq_t sum = 0;
        /* multiplicative binomial: comb = C(m,0) = 1; */
        seq_t comb = 1;

        for (idx_t k = 0; k < m; ++k) {
            /* comb == C(m,k) */
            seq_t tmp = comb * cache[k];
            tmp *= cache[m - k - 1];
            sum += tmp;

            /* advance comb to C(m, k+1) */
            comb = (comb * (m - k)) / (k + 1);
        }

        seq_t val = sum;
        if (m & 1)      /* (-1)^m = -1 when m odd */
            val -= 1;
        else            /* (+1) when m even */
            val += 1;
        cache[m] = val;
    }

    next_computed = n + 1;
    return cache[n];
}
