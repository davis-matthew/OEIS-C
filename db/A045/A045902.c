/*
 * A045902  Catafusenes (see reference for precise definition).
 * https://oeis.org/A045902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+1)*(n+4)*a(n) = (6*n^2+19*n+19)*a(n-1) -
 *            5*(n-2)*(n+2)*a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <vector>
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

#if defined(__cplusplus)
    /* C++ implementation */
    static std::vector<seq_t> cache;
    if (cache.empty()) {
        cache.resize(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
    }
    if (cache.size() <= n) {
        for (idx_t k = (idx_t)cache.size(); k <= n; ++k) {
            if constexpr (std::is_integral<seq_t>::value) {
                unsigned __int128 k128 = k;
                unsigned __int128 coeff1 = 6 * k128 * k128 + 19 * k128 + 19;
                unsigned __int128 coeff2 = 5 * (k128 - 2) * (k128 + 2);
                unsigned __int128 num = coeff1 * (unsigned __int128)cache[k - 1]
                                      - coeff2 * (unsigned __int128)cache[k - 2];
                unsigned __int128 denom = (k128 + 1) * (k128 + 4);
                cache.push_back((seq_t)(num / denom));
            } else {
                /* arbitrary‑precision seq_t: use its arithmetic operations */
                seq_t coeff1 = (seq_t)6 * (seq_t)k * (seq_t)k
                              + (seq_t)19 * (seq_t)k + (seq_t)19;
                seq_t coeff2 = (seq_t)5 * ((seq_t)k - (seq_t)2)
                              * ((seq_t)k + (seq_t)2);
                seq_t denom  = ((seq_t)k + (seq_t)1)
                              * ((seq_t)k + (seq_t)4);
                seq_t num = coeff1 * cache[k - 1] - coeff2 * cache[k - 2];
                cache.push_back(num / denom);
            }
        }
    }
    return cache[n];
#else
    /* C implementation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    if (cache == NULL) {
        cache = (seq_t *)calloc(2, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_size = 2;
    }
    if (cache_size <= n) {
        for (idx_t k = cache_size; k <= n; ++k) {
            unsigned __int128 k128 = k;
            unsigned __int128 coeff1 = 6 * k128 * k128 + 19 * k128 + 19;
            unsigned __int128 coeff2 = 5 * (k128 - 2) * (k128 + 2);
            unsigned __int128 num = coeff1 * (unsigned __int128)cache[k - 1]
                                  - coeff2 * (unsigned __int128)cache[k - 2];
            unsigned __int128 denom = (k128 + 1) * (k128 + 4);
            cache[k] = (seq_t)(num / denom);
        }
        cache_size = n + 1;
    }
    return cache[n];
#endif
}
