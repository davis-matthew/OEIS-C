/*
 * A170469  Number of reduced words of length n in Coxeter group on 28
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(n) = p(n) + 26 * Σ_{k=1}^{min(44,n)} a(n-k) – 351 * a(n-45)
   where p(0)=1, p(1..44)=2, p(45)=1, and p(n)=0 for n≥46.  This
   recurrence follows from the rational generating function of A170469. */

#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static size_t cache_len = 0;           /* number of terms already stored (0..len-1) */

/* resize the cache to hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_capacity)
        return;

    size_t new_cap = cache_capacity ? cache_capacity * 2 : needed;
    if (new_cap < needed)
        new_cap = needed;

#ifdef __cplusplus
    seq_t *newp = new seq_t[new_cap];
    if (cache) {
        std::copy(cache, cache + cache_len, newp);
        delete[] cache;
    } else {
        /* nothing to copy */
    }
    cache = newp;
#else
    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!newp) {
        abort();  /* memory exhausted */
    }
    if (cache) {
        memcpy(newp, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    cache = newp;
#endif
    cache_capacity = new_cap;
}

/* compute a(n) for arbitrary n >= 0 */
seq_t a(idx_t n)
{
    if ((idx_t)cache_len > n)
        return cache[n];

    /* grow the cache if necessary */
    ensure_cache_size(n);

    /* compute all missing terms up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 1;          /* p(0) = 1 */
            continue;
        }

        /* sum of the previous up to 44 terms */
        seq_t sum = 0;
        idx_t up = i < 44 ? i : 44;
        for (idx_t j = 1; j <= up; ++j)
            sum += cache[i - j];

        /* recurrence */
        seq_t val = sum * (seq_t)26;      /* 26 * sum */

        if (i >= 45)
            val -= (seq_t)351 * cache[i - 45];

        /* add the polynomial term p(i) */
        if (i <= 44)
            val += 2;
        else if (i == 45)
            val += 1;
        /* for i >= 46, p(i) = 0; no addition needed */

        cache[i] = val;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
