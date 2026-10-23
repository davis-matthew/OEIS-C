/*
 * A168910  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 41. For n ≥ 1: a(n) = 39 * Σ_{i=1}^{20} a(n-i)
 *            – 780 * a(n-21) (if n ≥ 21) + p(n), where p(n) = {1 for n = 0 or
 *            21, 2 for 1 ≤ n ≤ 20, 0 otherwise}. This follows from the
 *            rational generating function (t^21 + 2 t^20 + … + 2 t + 1) / (1
 *            – 39 t – 39 t^2 – … – 39 t^20 + 780 t^21)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cached a(n) values */
static size_t  cached_len = 0;   /* number of valid entries in cache */
static size_t  cached_cap  = 0;   /* allocated size of cache */

/* coefficients of the constant term in the recurrence */
static const seq_t p[22] = {
    1, /* n = 0 */
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, /* 1 ≤ n ≤ 20 */
    1  /* n = 21 */
};

static void grow_cache(size_t n)
{
    if (cached_cap >= n)
        return;
    size_t new_cap = n * 2 + 16;      /* a small growth factor */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();          /* out‑of‑memory: terminate */
    cache = new_cache;
    cached_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1U;

    /* Allocate cache if needed */
    if (cached_len == 0) {
        /* initial value a(0) */
        grow_cache(1);
        cache[0] = (seq_t)1U;
        cached_len = 1;
    }

    /* Ensure cache covers index n */
    if (cached_len <= n) {
        grow_cache(n + 1);
        /* Compute missing values up to n */
        for (idx_t i = cached_len; i <= n; ++i) {
            seq_t sum = (seq_t)0U;
            /* Σ_{j=1}^{20} a(i-j) */
            for (idx_t j = 1; j <= 20; ++j) {
                if (i >= j)
                    sum += cache[i - j];
                else
                    break;
            }
            seq_t res = (seq_t)39U * sum;          /* 39 * Σ a(i-j) */
            if (i >= 21)                           /* subtract 780*a(i-21) */
                res -= (seq_t)780U * cache[i - 21];
            if (i <= 21)                           /* add p(i) */
                res += p[i];
            cache[i] = res;
        }
        cached_len = n + 1;
    }

    return cache[n];
}
