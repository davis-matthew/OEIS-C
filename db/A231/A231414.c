/*
 * A231414  Number of (n+1)X(2+1) 0..2 arrays with no element equal to a
 *          strict majority of its horizontal, diagonal and antidiagonal
 *          neighbors, with values 0..2 introduced in row major order.
 * https://oeis.org/A231414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 17*a(n-1) - 28*a(n-2) - 119*a(n-3) +
 *            373*a(n-4) + 134*a(n-5) - 268*a(n-6) - 200*a(n-7) for n ≥ 8
 *            Initial values (1‑based): a(1)=71, a(2)=1032, a(3)=15125,
 *            a(4)=221445, a(5)=3245016, a(6)=47557773, a(7)=697029563
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#include <type_traits>
#endif

seq_t a(idx_t n) {
    /* cache[0] is unused; cache[1]–cache[7] hold the initial terms */
    static seq_t *cache = NULL;   /* allocated size in terms of idx_t indices */
    static idx_t cache_n = 0;     /* highest computed index so far */

    if (n == 0) return 0;         /* safe guard though offset is 1 */

    /* Ensure the cache contains all indices up to n */
    if (n > cache_n) {
        idx_t newSize = n;
        if (newSize < 7) newSize = 7;            /* at least store first 7 terms */

        if (cache == NULL) {
            /* Allocate a little extra buffer for safety */
            cache = (seq_t*)malloc((newSize + 2) * sizeof(seq_t));
            if (!cache) return 0;               /* out of memory */

            /* initial terms */
            cache[0] = 0;
            cache[1] = (seq_t)71;
            cache[2] = (seq_t)1032;
            cache[3] = (seq_t)15125;
            cache[4] = (seq_t)221445;
            cache[5] = (seq_t)3245016;
            cache[6] = (seq_t)47557773;
            cache[7] = (seq_t)697029563;
            cache_n = 7;
        } else if (newSize + 2 > cache_n + 2) {
            /* Grow cache if needed */
            seq_t *tmp = (seq_t*)realloc(cache, (newSize + 2) * sizeof(seq_t));
            if (!tmp) return 0;                 /* out of memory */
            cache = tmp;
        }

        /* Compute terms from max(cache_n+1, 8) up to n (recurrence only starts at 8) */
        idx_t start = cache_n + 1;
        if (start < 8) start = 8;

        for (idx_t i = start; i <= n; ++i) {
            seq_t val;
#if defined(__cplusplus)
            if constexpr (!std::is_class<seq_t>::value) {
                /* Use signed 128‑bit intermediate to avoid overflow */
                __int128 t = 0;
                t += (__int128)cache[(size_t)(i-1)] * 17;
                t -= (__int128)cache[(size_t)(i-2)] * 28;
                t -= (__int128)cache[(size_t)(i-3)] * 119;
                t += (__int128)cache[(size_t)(i-4)] * 373;
                t += (__int128)cache[(size_t)(i-5)] * 134;
                t -= (__int128)cache[(size_t)(i-6)] * 268;
                t -= (__int128)cache[(size_t)(i-7)] * 200;
                val = (seq_t)t;
            } else {
                /* For arbitrary‑precision class types, rely on overloaded operators */
                val = 17 * cache[(size_t)(i-1)]
                    - 28 * cache[(size_t)(i-2)]
                    - 119 * cache[(size_t)(i-3)]
                    + 373 * cache[(size_t)(i-4)]
                    + 134 * cache[(size_t)(i-5)]
                    - 268 * cache[(size_t)(i-6)]
                    - 200 * cache[(size_t)(i-7)];
            }
#else
            /* Plain C implementation – signed 128‑bit intermediate */
            __int128 t = 0;
            t += (__int128)cache[(size_t)(i-1)] * 17;
            t -= (__int128)cache[(size_t)(i-2)] * 28;
            t -= (__int128)cache[(size_t)(i-3)] * 119;
            t += (__int128)cache[(size_t)(i-4)] * 373;
            t += (__int128)cache[(size_t)(i-5)] * 134;
            t -= (__int128)cache[(size_t)(i-6)] * 268;
            t -= (__int128)cache[(size_t)(i-7)] * 200;
            val = (seq_t)t;
#endif
            cache[(size_t)i] = val;
        }
        cache_n = n;
    }

    return cache[(size_t)n];
}
