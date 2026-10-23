/*
 * A194238  Partial sums of A194237.
 * https://oeis.org/A194238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-16) - 2*a(n-17) + a(n-18) for n >
 *            18 Initial values: a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=5,
 *            a(6)=7, a(7)=10, a(8)=13, a(9)=16, a(10)=20, a(11)=24, a(12)=29,
 *            a(13)=35, a(14)=41, a(15)=48, a(16)=55, a(17)=62, a(18)=70
 * Range:     u8 n <= 34, u16 n <= 547, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* array of initial values a(0..18); a(0) unused */
    static const seq_t init[19] = {
        0ULL,      /* a(0) unused */
        0ULL,      /* a(1) */
        1ULL,      /* a(2) */
        2ULL,      /* a(3) */
        3ULL,      /* a(4) */
        5ULL,      /* a(5) */
        7ULL,      /* a(6) */
        10ULL,     /* a(7) */
        13ULL,     /* a(8) */
        16ULL,     /* a(9) */
        20ULL,     /* a(10) */
        24ULL,     /* a(11) */
        29ULL,     /* a(12) */
        35ULL,     /* a(13) */
        41ULL,     /* a(14) */
        48ULL,     /* a(15) */
        55ULL,     /* a(16) */
        62ULL,     /* a(17) */
        70ULL      /* a(18) */
    };

    if (n == 0) return 0;       /* safety for out‑of‑bounds indices */

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 1)           /* a(1) = 0 */
                cache[i] = 0;
            else if (i <= 18)     /* use the table for i <= 18 */
                cache[i] = init[i];
            else {
                seq_t val = 2 * cache[i-1]
                          -  cache[i-2]
                          +  cache[i-16]
                          - 2 * cache[i-17]
                          +  cache[i-18];
                cache[i] = val;
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
