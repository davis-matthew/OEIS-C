/*
 * A226312  Sum_{k=0..n} k*binomial(n,k)^2*binomial(2*k,k).
 * https://oeis.org/A226312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} k*binomial(n,k)^2*binomial(2*k,k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#include "../utils/binomial.h"
#else
#include "../utils/binomial.h"
#endif

seq_t a(idx_t n) {
#ifdef __cplusplus
    /* Use recurrence for integral seq_t, direct summation for big integers. */
    if constexpr (std::is_integral<seq_t>::value) {
        static seq_t *cache = nullptr;
        static size_t cache_size = 0;
        static size_t cache_cap = 0;

        if (cache_cap == 0) {
            cache_cap = 3;
            cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
            cache[0] = 0;
            cache[1] = 2;
            cache[2] = 20;
            cache_size = 3;
        }

        if (n < cache_size) return cache[n];

        if (n >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            while (new_cap <= n) new_cap <<= 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence (valid for i >= 3) */
            unsigned __int128 ii   = (unsigned __int128)i;
            unsigned __int128 im1  = ii - 1;
            unsigned __int128 im2  = ii - 2;
            unsigned __int128 coeff = (unsigned __int128)10 * ii * ii -
                                     (unsigned __int128)10 * ii + 3;
            unsigned __int128 num1 = im2 * coeff *
                                      (unsigned __int128)cache[i-1];
            unsigned __int128 num2 = (unsigned __int128)9 * im1 * im1 * im1 *
                                      (unsigned __int128)cache[i-2];
            unsigned __int128 numerator   = num1 - num2;
            unsigned __int128 denominator = im2 * ii * im1;
            cache[i] = (seq_t)(numerator / denominator);
        }

        cache_size = n + 1;
        return cache[n];
    } else {
        /* For big‑integer seq_t use the definition directly. */
        seq_t result = 0;
        for (idx_t k = 1; k <= n; ++k) {          /* k = 0 contributes 0 */
            seq_t b1 = binomial(n, k);
            seq_t b2 = b1 * b1;                  /* square */
            seq_t b3 = binomial(2 * k, k);
            seq_t term = b3 * b2 * (seq_t)k;
            result += term;
        }
        return result;
    }
#else
    /* C implementation – seq_t is a fixed‑size unsigned integer. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    if (cache_cap == 0) {
        cache_cap = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 20;
        cache_size = 3;
    }

    if (n < cache_size) return cache[n];

    if (n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        unsigned __int128 ii   = (unsigned __int128)i;
        unsigned __int128 im1  = ii - 1;
        unsigned __int128 im2  = ii - 2;
        unsigned __int128 coeff = (unsigned __int128)10 * ii * ii -
                                 (unsigned __int128)10 * ii + 3;
        unsigned __int128 num1 = im2 * coeff *
                                  (unsigned __int128)cache[i-1];
        unsigned __int128 num2 = (unsigned __int128)9 * im1 * im1 * im1 *
                                  (unsigned __int128)cache[i-2];
        unsigned __int128 numerator   = num1 - num2;
        unsigned __int128 denominator = im2 * ii * im1;
        cache[i] = (seq_t)(numerator / denominator);
    }

    cache_size = n + 1;
    return cache[n];
#endif
}
