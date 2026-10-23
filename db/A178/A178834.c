/*
 * A178834  a(n) counts anti-chains of size two in "0,1,2" Motzkin trees on n
 *          edges.
 * https://oeis.org/A178834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence * -(n-2)(n+2)a(n) + (4n²-n-8)a(n-1) +
 *            (2n²-n-12)a(n-2) * - 3n(4n-3)a(n-3) - 9n(n-1)a(n-4) = 0 *
 *            Initial values: a(0)=0, a(1)=0, a(2)=1, a(3)=5
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 39, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, exit, NULL */
#include <stddef.h>   /* size_t */

#ifdef __cplusplus
#include <vector>
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /*--- Storage --------------------------------------------*/
#ifdef __cplusplus
    /* C++: use std::vector for automatic construction */
    static std::vector<seq_t> cache_vec;
    static idx_t cached_up_to = (idx_t)-1;
    size_t new_cap = (n + 1) > 4 ? (size_t)(n + 1) : 4;
    if (cache_vec.empty()) {
        cache_vec.resize(new_cap);
        cache_vec[0] = (seq_t)0;
        cache_vec[1] = (seq_t)0;
        cache_vec[2] = (seq_t)1;
        cache_vec[3] = (seq_t)5;
        cached_up_to = 3;
    } else if (cache_vec.size() < new_cap) {
        cache_vec.resize(new_cap);
    }
#else
    /* C: raw malloc/realloc */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_up_to = (idx_t)-1;
    size_t new_cap = (n + 1) > 4 ? (size_t)(n + 1) : 4;
    if (!cache) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)5;
        cache_cap = new_cap;
        cached_up_to = 3;
    } else if (cache_cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = new_cap;
    }
#endif
    /*--- Compute missing terms ------------------------------*/
    if (cached_up_to < n) {
        for (idx_t i = cached_up_to + 1; i <= n; ++i) {
#ifdef __cplusplus
            if constexpr (std::is_integral<seq_t>::value) {
                using i128 = unsigned __int128;
                i128 ii = i;
                i128 t1 = 4*ii*ii - ii - 8;
                i128 t2 = 2*ii*ii - ii - 12;
                i128 c3 = 3*ii*(4*ii - 3);
                i128 c4 = 9*ii*(ii - 1);
                i128 denom = (ii - 2)*(ii + 2);
                i128 num = t1 * (i128)cache_vec[i-1] + t2 * (i128)cache_vec[i-2]
                           - c3 * (i128)cache_vec[i-3] - c4 * (i128)cache_vec[i-4];
                cache_vec[i] = (seq_t)(num / denom);
            } else {
                seq_t ii = (seq_t)i;
                seq_t t1 = (seq_t)4*ii*ii - ii - (seq_t)8;
                seq_t t2 = (seq_t)2*ii*ii - ii - (seq_t)12;
                seq_t c3 = (seq_t)3*ii*((seq_t)4*ii - (seq_t)3);
                seq_t c4 = (seq_t)9*ii*(ii - (seq_t)1);
                seq_t denom = (ii - (seq_t)2)*(ii + (seq_t)2);
                seq_t num = t1*cache_vec[i-1] + t2*cache_vec[i-2]
                           - c3*cache_vec[i-3] - c4*cache_vec[i-4];
                cache_vec[i] = num / denom;
            }
#else
            unsigned __int128 ii = i;
            unsigned __int128 t1 = 4*ii*ii - ii - 8;
            unsigned __int128 t2 = 2*ii*ii - ii - 12;
            unsigned __int128 c3 = 3*ii*(4*ii - 3);
            unsigned __int128 c4 = 9*ii*(ii - 1);
            unsigned __int128 denom = (ii - 2)*(ii + 2);
            unsigned __int128 num = t1 * (unsigned __int128)cache[(size_t)(i-1)]
                                  + t2 * (unsigned __int128)cache[(size_t)(i-2)]
                                  - c3 * (unsigned __int128)cache[(size_t)(i-3)]
                                  - c4 * (unsigned __int128)cache[(size_t)(i-4)];
            cache[(size_t)i] = (seq_t)(num / denom);
#endif
        }
        cached_up_to = n;
    }
    /*--- Return value ----------------------------------------*/
#ifdef __cplusplus
    return cache_vec[(size_t)n];
#else
    return cache[(size_t)n];
#endif
}
