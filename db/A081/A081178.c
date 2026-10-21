/*
 * A081178  a(0) = 1; for n>=1, a(n) = Sum_{k=0..n} 7^k*N(n,k), where
 *          N(n,k)=(1/n)*C(n,k)*C(n,k+1) are the Narayana numbers (A001263).
 * https://oeis.org/A081178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*(2*n-1)*a(n-1) - 36*(n-2)*a(n-2))/(n+1) for n>=2 a(0)
 *            = a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#  include <type_traits>
#endif

/* static cache for previously computed values */
static seq_t *cache = NULL;   /* array of terms */
static idx_t cached_n = 0;    /* highest index stored */
static idx_t cache_cap = 0;   /* allocated capacity */

/* grow the cache so that it can hold indices up to at least 'need' */
static void grow_cache(idx_t need)
{
    if (cache_cap >= need)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    if (new_cap < need)
        new_cap = need;
    /* realloc may return NULL, which is acceptable if new_cap==0 */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cached_n = 1;
    }

    if (n <= cached_n)
        return cache[n];

    /* ensure sufficient space */
    grow_cache(n + 1);

    for (idx_t i = cached_n + 1; i <= n; ++i) {
#ifdef __cplusplus
        /* C++ build */
        if constexpr (std::is_integral<seq_t>::value) {
            /* use 128‑bit intermediate to avoid overflow */
            unsigned __int128 num = (unsigned __int128)8 *
                                    (unsigned __int128)(2 * i - 1) *
                                    (unsigned __int128)cache[i - 1];
            num -= (unsigned __int128)36 *
                   (unsigned __int128)(i - 2) *
                   (unsigned __int128)cache[i - 2];
            cache[i] = (seq_t)(num / (unsigned __int128)(i + 1));
        } else {
            /* arbitrary‑precision type: use normal arithmetic operators */
            seq_t term1 = cache[i - 1];
            seq_t term2 = cache[i - 2];
            cache[i] = ((term1 * (seq_t)8 * (seq_t)(2 * i - 1))
                        - (term2 * (seq_t)36 * (seq_t)(i - 2)))
                       / (seq_t)(i + 1);
        }
#else
        /* C build */
#  if defined(__SIZEOF_INT128__)
        unsigned __int128 num = (unsigned __int128)8 *
                                (unsigned __int128)(2 * i - 1) *
                                (unsigned __int128)cache[i - 1];
        num -= (unsigned __int128)36 *
               (unsigned __int128)(i - 2) *
               (unsigned __int128)cache[i - 2];
        cache[i] = (seq_t)(num / (unsigned __int128)(i + 1));
#  else
        /* fall back to normal arithmetic (may overflow if seq_t is builtin) */
        seq_t term1 = cache[i - 1];
        seq_t term2 = cache[i - 2];
        cache[i] = ((term1 * (seq_t)8 * (seq_t)(2 * i - 1))
                    - (term2 * (seq_t)36 * (seq_t)(i - 2)))
                   / (seq_t)(i + 1);
#  endif
#endif
    }

    cached_n = n;
    return cache[n];
}
