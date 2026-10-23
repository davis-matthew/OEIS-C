/*
 * A010925  Pisot sequence T(5,21), a(n) = floor( a(n-1)^2/a(n-2) ).
 * https://oeis.org/A010925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=5, a(1)=21, a(n)=floor(a(n-1)^2 / a(n-2)) for n>=2
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#include <type_traits>
#endif
#include <stdlib.h>

static seq_t mul_div_floor(seq_t a, seq_t b) {
#if defined(__cplusplus) && defined(__cpp_if_constexpr)
    if constexpr (std::is_integral<seq_t>::value) {
#endif
#if defined(__SIZEOF_INT128__)
        unsigned __int128 prod = (unsigned __int128)a * (unsigned __int128)a;
        return (seq_t)(prod / (unsigned __int128)b);
#else
        return (seq_t)(((unsigned long long)a * (unsigned long long)a) / (unsigned long long)b);
#endif
#if defined(__cplusplus) && defined(__cpp_if_constexpr)
    } else {
        return (seq_t)(a * a / b);
    }
#endif
}

#ifdef __cplusplus
static std::vector<seq_t> cache_vec;
#else
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)21;

#ifdef __cplusplus
    if (n < cache_vec.size()) return cache_vec[n];
    size_t old_size = cache_vec.size();
    cache_vec.resize(n + 1);
    if (old_size == 0) {
        cache_vec[0] = (seq_t)5;
        cache_vec[1] = (seq_t)21;
        old_size = 2;
    }
    for (size_t i = old_size; i <= n; ++i) {
        cache_vec[i] = mul_div_floor(cache_vec[i-1], cache_vec[i-2]);
    }
    return cache_vec[n];
#else
    if (n < cache_cap) return cache[n];
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    if (cache_cap == 0) {
        cache[0] = (seq_t)5;
        if (new_cap > 1) cache[1] = (seq_t)21;
        for (idx_t i = 2; i < new_cap; ++i) {
            cache[i] = mul_div_floor(cache[i-1], cache[i-2]);
        }
    } else {
        for (idx_t i = cache_cap; i < new_cap; ++i) {
            cache[i] = mul_div_floor(cache[i-1], cache[i-2]);
        }
    }
    cache_cap = new_cap;
    return cache[n];
#endif
}
