/*
 * A170358  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; * For all n>=0: * p_n = 1 if n==0 or n==43 * p_n = 2 if
 *            1<=n<=42 * p_n = 0 otherwise * let m = min(n,42) * a(n) = p_n +
 *            11*sum_{k=1}^{m} a(n-k) * - (n>=43 ? 66*a(n-43) : 0) * * The
 *            coefficients come from the generating function * (t^43 + 2*t^42
 *            + ... + 2*t + 1) / (66*t^43 - 11*t^42 - ......
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifndef __cplusplus
#include <stdint.h>
#endif

#if defined(__cplusplus)
#include <vector>
#endif

/* static cache of computed terms (C mode: plain array, C++ mode: std::vector) */
static idx_t cached_len = 0;          /* highest index that has been computed */

#ifndef __cplusplus
static seq_t *cache = NULL;           /* C implementation uses realloc */
static void ensure_cache(idx_t n)
{
    /* already computed? */
    if (cached_len >= n && cache != NULL)
        return;

    /* allocate / enlarge storage */
    seq_t *new_cache = realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    cache = new_cache;

    /* ensure a(0) is available if this is the first allocation */
    if (cached_len == 0 && n >= 0)
        cache[0] = (seq_t)1;

    /* compute missing terms */
    for (idx_t i = cached_len + 1; i <= n; ++i) {
        seq_t p = (i == 0 || i == 43) ? (seq_t)1
               : (i <= 42) ? (seq_t)2 : (seq_t)0;
        seq_t val = p;

        idx_t upper = (i < 43 ? i : 42);
        for (idx_t j = 1; j <= upper; ++j)
            val = val + (seq_t)11 * cache[i - j];

        if (i >= 43)
            val = val - (seq_t)66 * cache[i - 43];

        cache[i] = val;
    }

    cached_len = n;
}
#else
static std::vector<seq_t> cache_vec;   /* C++ implementation uses std::vector */
static void ensure_cache(idx_t n)
{
    if (cache_vec.size() > n)
        return;

    size_t old_size = cache_vec.size();
    cache_vec.resize(n + 1);

    for (idx_t i = old_size; i <= n; ++i) {
        seq_t p = (i == 0 || i == 43) ? (seq_t)1
               : (i <= 42) ? (seq_t)2 : (seq_t)0;
        seq_t val = p;

        idx_t upper = (i < 43 ? i : 42);
        for (idx_t j = 1; j <= upper; ++j)
            val = val + (seq_t)11 * cache_vec[i - j];

        if (i >= 43)
            val = val - (seq_t)66 * cache_vec[i - 43];

        cache_vec[i] = val;
    }
}
#endif

seq_t a(idx_t n)
{
    ensure_cache(n);
#ifndef __cplusplus
    return cache[n];
#else
    return cache_vec[n];
#endif
}
