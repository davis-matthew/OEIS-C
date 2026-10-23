/*
 * A179539  a(0) = 1, a(1) = 0, a(n) = 2*n*(a(n-1) + a(n-2)), n > 1.
 * https://oeis.org/A179539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(a(n-1) + a(n-2)); a(0) = 1; a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* static cache for repeated calls (any order) */
static seq_t *cache_c = NULL;      /* C implementation */
static size_t cache_len_c = 0;
#ifdef __cplusplus
static std::vector<seq_t> cache_cpp;   /* C++ implementation */
#endif

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

#ifdef __cplusplus
    /* C++: use std::vector for automatic memory management */
    if (cache_cpp.size() <= n)
    {
        cache_cpp.resize(n + 1);
        if (cache_cpp.size() >= 2)
        {
            cache_cpp[0] = (seq_t)1;
            cache_cpp[1] = (seq_t)0;
        }
        for (size_t i = 2; i <= n; ++i)
        {
            cache_cpp[i] = (seq_t)2 * (seq_t)i * (cache_cpp[i - 1] + cache_cpp[i - 2]);
        }
    }
    return cache_cpp[(size_t)n];
#else
    /* C: use malloc/realloc and a trivial cache */
    if (cache_len_c <= (size_t)n)
    {
        size_t old_len = cache_len_c;
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache_c, new_len * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        cache_c = tmp;
        cache_len_c = new_len;

        /* initialise missing initial values if necessary */
        if (old_len == 0)
        {
            cache_c[0] = (seq_t)1;
            if (new_len > 1) cache_c[1] = (seq_t)0;
            old_len = 2;
        }
        else if (old_len == 1)
        {
            if (new_len > 1) cache_c[1] = (seq_t)0;
            old_len = 2;
        }

        for (size_t i = old_len; i <= (size_t)n; ++i)
        {
            cache_c[i] = (seq_t)2 * (seq_t)i * (cache_c[i - 1] + cache_c[i - 2]);
        }
    }
    return cache_c[(size_t)n];
#endif
}
