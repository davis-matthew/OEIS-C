/*
 * A020746  Pisot sequence T(3,7), a(n) = floor(a(n-1)^2/a(n-2)).
 * https://oeis.org/A020746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( a(n-1)^2 / a(n-2) ) Recurrence: a(0) = 3, a(1) = 7
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 53, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <vector>
#include <type_traits>
#endif

/* ------------------------------------------------------------------ */
/* Helper: integer division without overflow ().                     */
/* The implementation is the same for C and C++, but uses a
   different technique for integral types on C++.                    */
/* ------------------------------------------------------------------ */
static inline seq_t mul_div(seq_t prev, seq_t prev2)
{
#ifdef __cplusplus
    if constexpr (std::is_integral_v<seq_t>)
        return (seq_t)(((unsigned __int128)prev * prev) / prev2);
    else
        return (prev * prev) / prev2;            /* arbitrary‑precision case */
#else
    return (seq_t)(((__uint128_t)prev * prev) / prev2);
#endif
}

/* ------------------------------------------------------------------ */
/* Cache handling: C++ uses std::vector, C uses malloc/realloc.      */
/* ------------------------------------------------------------------ */
#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
#else
static seq_t *cache = NULL;
static idx_t cache_size = 0;
#endif

static inline void ensure_capacity(idx_t n)
{
#ifdef __cplusplus
    if ((idx_t)cache_vec.size() <= n) {
        idx_t new_cap = cache_vec.size() == 0 ? 1 : cache_vec.size() * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        cache_vec.resize(new_cap);
        /* initialise new entries to zero */
        for (idx_t i = cache_vec.size(); i < new_cap; ++i)
            cache_vec[i] = (seq_t)0;
    }
#else
    if (n + 1 > cache_size) {
        idx_t new_cap = cache_size == 0 ? 1 : cache_size * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                       /* allocation failure */
        /* zero‑initialize new elements */
        for (idx_t i = cache_size; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_size = new_cap;
    }
#endif
}

/* ------------------------------------------------------------------ */
/* The required function                                        */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* base values are fixed constants */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)7;

    ensure_capacity(n);

#ifdef __cplusplus
    /* Ensure first two entries are set (vector may initially be empty) */
    if (cache_vec[0] == (seq_t)0) cache_vec[0] = (seq_t)3;
    if (cache_vec[1] == (seq_t)0) cache_vec[1] = (seq_t)7;

    /* Compute up to n from the first missing index */
    idx_t start = cache_vec.size() > 2 ? 2 : cache_vec.size();
    for (idx_t i = start; i <= n; ++i) {
        if (cache_vec[i] == (seq_t)0) {
            seq_t prev  = cache_vec[i - 1];
            seq_t prev2 = cache_vec[i - 2];
            cache_vec[i] = mul_div(prev, prev2);
        }
    }
    return cache_vec[n];
#else
    if (cache[0] == (seq_t)0) cache[0] = (seq_t)3;
    if (cache[1] == (seq_t)0) cache[1] = (seq_t)7;

    idx_t start = cache_size > 2 ? 2 : cache_size;
    for (idx_t i = start; i <= n; ++i) {
        if (cache[i] == (seq_t)0) {
            seq_t prev  = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            cache[i] = mul_div(prev, prev2);
        }
    }
    return cache[n];
#endif
}
