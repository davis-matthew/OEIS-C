/*
 * A202303  Drop the last digit of A023110(n).
 * https://oeis.org/A202303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 26, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1443*a(n-7) - 1443*a(n-14) + a(n-21)   (for n >= 23)
 * Initial values:
 *   a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 *   a(5)=1,  a(6)=4,  a(7)=16, a(8)=25,
 *   a(9)=36, a(10)=144, a(11)=324, a(12)=1849,
 *   a(13)=6400, a(14)=23716, a(15)=36481,
 *   a(16)=51984, a(17)=207936, a(18)=467856,
 *   a(19)=2666689, a(20)=9229444, a(21)=34199104,
 *   a(22)=52606009
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cached results for memoisation (1-based indices). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Handle indexes before the defined offset. */
    if (n == 0)
        return (seq_t)0;

    /* First 22 terms are given explicitly. */
    static const seq_t init[22] = {
        (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
        (seq_t)1, (seq_t)4, (seq_t)16, (seq_t)25,
        (seq_t)36, (seq_t)144, (seq_t)324, (seq_t)1849,
        (seq_t)6400, (seq_t)23716, (seq_t)36481,
        (seq_t)51984, (seq_t)207936, (seq_t)467856,
        (seq_t)2666689, (seq_t)9229444, (seq_t)34199104,
        (seq_t)52606009
    };

    /* Grow the cache to accommodate requested index n. */
    if ((size_t)n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp && n > 0)
            return (seq_t)0;          /* allocation failure */
        cache = tmp;

        size_t start = cache_size + 1;          /* indices to compute (1‑based) */

        for (idx_t i = start; i <= n; ++i) {
            if ((idx_t)i <= 22) {
                /* Copy the explicitly known initial values. */
                cache[(size_t)i - 1] = init[(size_t)i - 1];
            } else {
                /* Recurrence: a(n) = 1443*a(n-7) - 1443*a(n-14) + a(n-21) */
                seq_t term = (seq_t)1443 * cache[(size_t)(i - 8)]   /* a(i-7)  */
                           - (seq_t)1443 * cache[(size_t)(i - 15)]  /* a(i-14) */
                           + cache[(size_t)(i - 22)];              /* a(i-21) */
                cache[(size_t)i - 1] = term;
            }
        }
        cache_size = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
