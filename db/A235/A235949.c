/*
 * A235949  Number of (n+1) X (3+1) 0..2 arrays colored with the upper median
 *          value of each 2 X 2 subblock.
 * https://oeis.org/A235949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 14, u32 n <= 46, u64 n <= 110, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A235949
   Empirical recurrence:  a(n) = 4*a(n-4)  for n > 13,
   with initial values a(1)=905, a(2)=1372, a(3)=1494, a(4)=1780,
   a(5)=2478, a(6)=3504, a(7)=4884, a(8)=6888, a(9)=9816,
   a(10)=13920, a(11)=19554, a(12)=27600, a(13)=39296. */

#include <stdlib.h>

/* Forward declaration of any utils not used */

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* base values for n=1..13 */
    static const seq_t base_vals[14] = {
        0,          /* unused zero index */
        905, 1372, 1494, 1780, /* 1..4 */
        2478, 3504, 4884, 6888, /* 5..8 */
        9816, 13920, 19554, 27600, 39296 /* 9..13 */
    };

    /* ensure cache big enough */
    if (cache_size < n + 1) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure, return 0 */
        cache = new_cache;
        /* initialize newly allocated part */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i <= 13) {
                cache[i] = base_vals[i];
            } else {
                /* compute using recurrence 4*a(i-4) */
                cache[i] = cache[i - 4] * ((seq_t)4);
            }
        }
        cache_size = new_size;
    } else {
        /* compute missing values if any */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i <= 13) {
                cache[i] = base_vals[i];
            } else {
                cache[i] = cache[i - 4] * ((seq_t)4);
            }
        }
        cache_size = n + 1;
    }

    return cache[n];
}
