/*
 * A165515  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of t^n in (t^9 + 2 t^8 + … + 2 t + 1) / (406
 *            t^9 - 28 t^8 - … - 28 t + 1). The sequence satisfies a(n) = 28 *
 *            (a(n-1)+…+a(n-8)) - 406 * a(n-9) + b(n), where b(0)=1, b(1…8)=2,
 *            b(9)=1, b(n)=0 for n≥10
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#  include <vector>
#  include <cstddef>
#  include <type_traits>
#else
#  include <stdlib.h>
#  include <stddef.h>
#endif
#include "../utils/mul_small.h"

/* ------------  Public entry point ------------------------------------ */
seq_t a(idx_t n)
{
    /* --------- C implementation ----------------------------------- */
#if !defined(__cplusplus)
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    /* Grow cache if needed */
    if ((size_t)n >= cache_size) {
        cache = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            /* add 28 * a(i-j) for j = 1..8 */
            for (int j = 1; j <= 8; ++j) {
                if (i >= (size_t)j)
                    sum += mul_small(cache[i - j], (unsigned int)28);
            }
            /* subtract 406 * a(i-9) if applicable */
            if (i >= 9)
                sum -= mul_small(cache[i - 9], (unsigned int)406);
            /* add constant from b(i) */
            if (i == 0)          /* never happens here */
                sum += (seq_t)1;
            else if (i <= 8)
                sum += (seq_t)2;
            else if (i == 9)
                sum += (seq_t)1;
            /* else b(i)=0 */
            cache[i] = sum;
        }
        cache_size = (size_t)n + 1;
    }
    return cache[(size_t)n];
#endif

    /* --------- C++ implementation --------------------------------- */
#if defined(__cplusplus)
    /* Use a vector to store computed values dynamically.  */
    static std::vector<seq_t> cache;
    if (cache.size() <= n) {
        size_t cur = cache.size();
        cache.resize((size_t)n + 1);        /* extend storage */
        for (size_t i = cur; i <= (size_t)n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
                continue;
            }
            seq_t sum = (seq_t)0;
            for (int j = 1; j <= 8; ++j) {
                if (i >= (size_t)j)
                    sum += mul_small(cache[i - j], (unsigned int)28);
            }
            if (i >= 9)
                sum -= mul_small(cache[i - 9], (unsigned int)406);
            if (i <= 8)
                sum += (seq_t)2;
            else if (i == 9)
                sum += (seq_t)1;
            cache[i] = sum;
        }
    }
    return cache[(size_t)n];
#endif
}
