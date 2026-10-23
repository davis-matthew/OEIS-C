/*
 * A181255  Number of (n+2) X 3 binary matrices with every 3 X 3 block having
 *          exactly four 1's.
 * https://oeis.org/A181255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 12*a(n-3) - 36*a(n-4) - 27*a(n-6) + 81*a(n-7)
 *            Initial values: a(1)=126, a(2)=336, a(3)=906, a(4)=2484,
 *            a(5)=7218, a(6)=21024, a(7)=61398
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <type_traits>
template <typename T>
inline T mul_small(T x, unsigned m) {
    if constexpr (std::is_integral_v<T>)
        return (T)((unsigned __int128)x * m);
    else
        return x * m;
}
template <typename T>
inline T compute_term(const T &a1, const T &a3, const T &a4,
                      const T &a6, const T &a7) {
    T sum = mul_small(a1,3) + mul_small(a3,12) + mul_small(a7,81);
    T diff = mul_small(a4,36) + mul_small(a6,27);
    return sum - diff;
}
#else
static inline seq_t mul_small(seq_t x, unsigned m) {
    return (seq_t)((unsigned __int128)x * m);
}
static inline seq_t compute_term(seq_t a1, seq_t a3, seq_t a4,
                                 seq_t a6, seq_t a7) {
    seq_t sum = mul_small(a1,3) + mul_small(a3,12) + mul_small(a7,81);
    seq_t diff = mul_small(a4,36) + mul_small(a6,27);
    return sum - diff;
}
#endif

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (cache_size == 0) {
        cache_size = 7;
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failure */
        cache[0] = 0;
        cache[1] = 126;
        cache[2] = 336;
        cache[3] = 906;
        cache[4] = 2484;
        cache[5] = 7218;
        cache[6] = 21024;
        cache[7] = 61398;
    } else if (n > cache_size) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 7) continue; /* already set */
            cache[i] = compute_term(cache[i-1], cache[i-3],
                                    cache[i-4], cache[i-6], cache[i-7]);
        }
        cache_size = n;
    }
    return cache[n];
}
