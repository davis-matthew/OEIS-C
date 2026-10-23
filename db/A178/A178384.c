/*
 * A178384  A (-1,1) Somos-4 sequence associated with the elliptic curve y^2 +
 *          y = x^3 + x.
 * https://oeis.org/A178384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-a(n-1)*a(n-3) + a(n-2)^2) / a(n-4) Base: a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=3
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 16, i64 n <= 23, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
#include <type_traits>
#endif

/* Static cache for memoization */
static seq_t *cache = NULL;
static idx_t cache_len = 0;      /* number of terms currently computed */
static size_t cache_cap = 0;     /* allocated capacity */

/* Multiply and other helpers that use __int128 for integral seq_t types,
   otherwise use the normal operators. */
#ifdef __cplusplus
static inline seq_t mul128(seq_t a, seq_t b) {
    if constexpr (std::is_integral<seq_t>::value) {
        __int128 r = (__int128)a * (__int128)b;
        return (seq_t)r;
    } else {
        return a * b;
    }
}
static inline seq_t sub128(seq_t a, seq_t b) {
    if constexpr (std::is_integral<seq_t>::value) {
        __int128 r = (__int128)a - (__int128)b;
        return (seq_t)r;
    } else {
        return a - b;
    }
}
static inline seq_t div128(seq_t a, seq_t b) {
    if constexpr (std::is_integral<seq_t>::value) {
        __int128 r = (__int128)a / (__int128)b;
        return (seq_t)r;
    } else {
        return a / b;
    }
}
#else
static inline seq_t mul128(seq_t a, seq_t b) {
    __int128 r = (__int128)a * (__int128)b;
    return (seq_t)r;
}
static inline seq_t sub128(seq_t a, seq_t b) {
    __int128 r = (__int128)a - (__int128)b;
    return (seq_t)r;
}
static inline seq_t div128(seq_t a, seq_t b) {
    __int128 r = (__int128)a / (__int128)b;
    return (seq_t)r;
}
#endif

/* Ensure cache holds terms up to index n */
static void
ensure_cache(idx_t n)
{
    if (n < cache_len) return;

    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        while (newcap < needed) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = newcap;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) cache[(size_t)i] = 1;
        else if (i == 1) cache[(size_t)i] = 1;
        else if (i == 2) cache[(size_t)i] = 1;
        else if (i == 3) cache[(size_t)i] = 3;
        else {
#ifdef __cplusplus
            seq_t a1 = cache[(size_t)(i - 1)];
            seq_t a2 = cache[(size_t)(i - 2)];
            seq_t a3 = cache[(size_t)(i - 3)];
            seq_t a4 = cache[(size_t)(i - 4)];
            seq_t t1 = mul128(a1, a3);
            seq_t t2 = mul128(a2, a2);
            seq_t num = sub128(t2, t1);
            seq_t val = div128(num, a4);
            cache[(size_t)i] = val;
#else
            __int128 a1 = cache[(size_t)(i - 1)];
            __int128 a2 = cache[(size_t)(i - 2)];
            __int128 a3 = cache[(size_t)(i - 3)];
            __int128 a4 = cache[(size_t)(i - 4)];
            __int128 num = -a1 * a3 + a2 * a2;
            cache[(size_t)i] = (seq_t)(num / a4);
#endif
        }
    }
    cache_len = n + 1;
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
