/*
 * A209954  Number of (n+1)X3 0..2 arrays with every 2X2 subblock having one,
 *          three or four distinct clockwise edge differences.
 * https://oeis.org/A209954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1) -23*a(n-2) -678*a(n-3) -472*a(n-4) +1196*a(n-5)
 *            +448*a(n-6) -416*a(n-7) (n>7) Initial values: a(1)=567,
 *            a(2)=12161, a(3)=260201, a(4)=5569307, a(5)=119198607,
 *            a(6)=2551206049, a(7)=54603359537
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* highest index computed */
    static size_t cache_cap = 0;  /* allocated capacity, in seq_t elements */

    /* Ensure there is space for 0..n and at least 0..7 */
    size_t needed = (size_t)n + 1;
    if (needed < 8) needed = 8;
    if (needed > cache_cap) {
#ifdef __cplusplus
        /* allocate a new array and copy old contents */
        seq_t *tmp = new (std::nothrow) seq_t[needed];
        if (!tmp) return 0; /* allocation failure: result undefined */
        if (cache) {
            for (idx_t i = 0; i <= (idx_t)cache_len; ++i) tmp[i] = cache[i];
            delete[] cache;
        }
        cache = tmp;
#else
        seq_t *tmp = realloc(cache, needed * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
#endif
        cache_cap = needed;
    }

    /* initialize the first seven terms if not already done */
    if (cache_len < 7) {
        static const seq_t init[8] = {
            0,                  /* unused index 0 */
            (seq_t)567,
            (seq_t)12161,
            (seq_t)260201,
            (seq_t)5569307,
            (seq_t)119198607,
            (seq_t)2551206049,
            (seq_t)54603359537
        };
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = init[i];
        cache_len = 7;
    }

    /* compute remaining terms up to n using the recurrence */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val = (seq_t)24 * cache[i-1]
                    - (seq_t)23 * cache[i-2]
                    - (seq_t)678 * cache[i-3]
                    - (seq_t)472 * cache[i-4]
                    + (seq_t)1196 * cache[i-5]
                    + (seq_t)448 * cache[i-6]
                    - (seq_t)416 * cache[i-7];
        cache[i] = val;
    }
    if (n > cache_len) cache_len = n;

    return cache[n];
}
