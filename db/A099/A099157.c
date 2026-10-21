/*
 * A099157  a(n) = 4^(n-1)*U(n-1, 3/2) where U is the Chebyshev polynomial of
 *          the second kind.
 * https://oeis.org/A099157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*a(n-1)-16*a(n-2); a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A099157.  The recurrence is linear of
   order two; an incremental memoised implementation is used
   so that repeated calls with arbitrary n are fast. */
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
#include <type_traits>
/* Helper multiplications that use 128‑bit arithmetic when seq_t
   is the built‑in 64‑bit unsigned type.  Otherwise plain
   multiplication is used, which is safe for big‑int types. */
template <typename T>
static inline T mul12(const T &x) {
    if constexpr (std::is_trivially_copyable<T>::value &&
                  std::is_unsigned<T>::value &&
                  sizeof(T) == 8) {
        return static_cast<T>((__uint128_t)x * (unsigned)12);
    } else {
        return x * (T)12;
    }
}
template <typename T>
static inline T mul16(const T &x) {
    if constexpr (std::is_trivially_copyable<T>::value &&
                  std::is_unsigned<T>::value &&
                  sizeof(T) == 8) {
        return static_cast<T>((__uint128_t)x * (unsigned)16);
    } else {
        return x * (T)16;
    }
}
#else
/* In plain C we are guaranteed that seq_t is a builtin unsigned type
   (any big‑int build uses C++).  Therefore 128‑bit casts are safe. */
static inline seq_t mul12(seq_t x) {
    return (seq_t)((__uint128_t)x * (unsigned)12);
}
static inline seq_t mul16(seq_t x) {
    return (seq_t)((__uint128_t)x * (unsigned)16);
}
#endif

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;     /* out of memory, unlikely */

        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
            free(cache);
        }

        /* initialise existing entries if cache was empty */
        if (cache_size == 0) new_cache[0] = (seq_t)0;
        if (cache_size <= 1) {
            new_cache[1] = (seq_t)1;
            cache_size = 2;
        }

        /* compute new terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev1 = new_cache[i - 1];
            seq_t prev2 = new_cache[i - 2];
            new_cache[i] = mul12(prev1) - mul16(prev2);
        }
        cache_size = new_size;
        cache = new_cache;
    }
    return cache[n];
}
