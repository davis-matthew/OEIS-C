/*
 * A016092  Expansion of 1/((1-8*x)*(1-9*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A016092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=40, a(2)=1005, a(3)=20300, * a(n) = 40*a(n-1) -
 *            595*a(n-2) + 3900*a(n-3) - 9504*a(n-4) for n≥4
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <type_traits>
#endif

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with the first four terms if empty */
    if (cache_len == 0) {
        cache_len = 4;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)40;
        cache[2] = (seq_t)1005;
        cache[3] = (seq_t)20300;
    }

    /* Extend cache if needed */
    if (cache_len <= n) {
        /* Allocate space up to n */
        idx_t new_len = n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

        /* Compute terms starting from the first missing index */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t term;
#ifdef __cplusplus
            /* In C++ we can detect if seq_t is an integral type */
            if constexpr (std::is_integral<seq_t>::value) {
                /* Use 128‑bit arithmetic to avoid overflow */
                signed __int128 sum =
                    (signed __int128)cache[i-1] * 40
                  - (signed __int128)cache[i-2] * 595
                  + (signed __int128)cache[i-3] * 3900
                  - (signed __int128)cache[i-4] * 9504;
                term = (seq_t)sum;
            } else {
                /* arbitrary‑precision type */
                term = cache[i-1] * 40
                     - cache[i-2] * 595
                     + cache[i-3] * 3900
                     - cache[i-4] * 9504;
            }
#else
            /* Pure C: seq_t is an integer type */
            signed __int128 sum =
                (signed __int128)cache[i-1] * 40
              - (signed __int128)cache[i-2] * 595
              + (signed __int128)cache[i-3] * 3900
              - (signed __int128)cache[i-4] * 9504;
            term = (seq_t)sum;
#endif
            cache[i] = term;
        }
        cache_len = new_len;
    }

    return cache[n];
}
