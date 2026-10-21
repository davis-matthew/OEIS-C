/*
 * A231297  Number of (n+1)X(3+1) 0..2 arrays with no element unequal to a
 *          strict majority of its horizontal, vertical and antidiagonal
 *          neighbors, with values 0..2 introduced in row major order.
 * https://oeis.org/A231297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 17*a(n-2) - 23*a(n-3) - 78*a(n-4) - 11*a(n-5)
 *            + 143*a(n-6) + 431*a(n-7) - 362*a(n-8) - 763*a(n-9) +
 *            176*a(n-10) - 60*a(n-11) + 336*a(n-12) + 448*a(n-13) -
 *            256*a(n-14) (n>16) with a(1)=4, a(2)=16, a(3)=69, a(4)=301,
 *            a(5)=1414, a(6)=6850, a(7)=33316, a(8)=162796, a(9)=797293,
 *            a(10...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <type_traits>
#endif

/* Initial 16 values */
static const seq_t init_vals[16] = {
    4ULL, 16ULL, 69ULL, 301ULL, 1414ULL, 6850ULL, 33316ULL,
    162796ULL, 797293ULL, 3906788ULL, 19152603ULL, 93902795ULL,
    460455941ULL, 2257842932ULL, 11071788203ULL, 54292311074ULL
};

/* Coefficients for recurrence */
static const int coeffs[14] = {
    3, 17, -23, -78, -11, 143, 431, -362, -763, 176,
    -60, 336, 448, -256
};

/* Cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static size_t cache_size = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Ensure cache has enough space */
    if (cache_capacity < (size_t)n) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 16;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 16) {
            cache[i - 1] = init_vals[i - 1];
        } else {
#ifdef __cplusplus
            if constexpr (std::is_integral<seq_t>::value) {
                unsigned __int128 pos = 0, neg = 0;
                for (int k = 0; k < 14; ++k) {
                    seq_t v = cache[i - 2 - k];
                    int c = coeffs[k];
                    if (c > 0) {
                        pos += (unsigned __int128)v * (unsigned int)c;
                    } else {
                        neg += (unsigned __int128)v * (unsigned int)(-c);
                    }
                }
                cache[i - 1] = (seq_t)((unsigned long long)(pos - neg));
            } else {
                seq_t pos = 0, neg = 0;
                for (int k = 0; k < 14; ++k) {
                    seq_t v = cache[i - 2 - k];
                    int c = coeffs[k];
                    if (c > 0) {
                        pos += v * (seq_t)c;
                    } else {
                        neg += v * (seq_t)(-c);
                    }
                }
                cache[i - 1] = pos - neg;
            }
#else
            unsigned __int128 pos = 0, neg = 0;
            for (int k = 0; k < 14; ++k) {
                seq_t v = cache[i - 2 - k];
                int c = coeffs[k];
                if (c > 0) {
                    pos += (unsigned __int128)v * (unsigned int)c;
                } else {
                    neg += (unsigned __int128)v * (unsigned int)(-c);
                }
            }
            cache[i - 1] = (seq_t)((unsigned long long)(pos - neg));
#endif
        }
    }
    cache_size = n;
    return cache[n - 1];
}
