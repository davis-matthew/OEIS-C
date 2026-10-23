/*
 * A186586  Number of (n+3)X9 0..2 arrays with every 4X4 subblock commuting
 *          with each horizontal and vertical neighbor 4X4 subblock.
 * https://oeis.org/A186586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 35, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Formula (empirical linear recurrence, valid for n > 16):
   a(n) = 2*a(n-1) - a(n-2)
        + 2*a(n-4) - 4*a(n-5)
        + 2*a(n-6) + 2*a(n-8) - 2*a(n-9)

   Initial values (offset 1):
   a(1)=1728546,  a(2)=79015,  a(3)=149305,  a(4)=328891,
   a(5)=626233,  a(6)=811795,  a(7)=823249,  a(8)=841075,
   a(9)=880969,  a(10)=1019683, a(11)=1221265, a(12)=1504507,
   a(13)=2353993, a(14)=3537235, a(15)=4869121, a(16)=7110411
*/

#include <stdlib.h>

static const seq_t init[16] = {
    1728546, 79015, 149305, 328891,
    626233, 811795, 823249, 841075,
    880969, 1019683, 1221265, 1504507,
    2353993, 3537235, 4869121, 7110411
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* value for index 0 is unused but safe */

    /* static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* highest n that is currently cached (>=0) */

    /* extend cache if necessary */
    if (n > cache_len) {
        /* allocate a new buffer big enough for indices [0..n] */
        seq_t *new_cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (!new_cache) abort();      /* allocation failure – terminate */

        /* copy existing cached part (if any) */
        if (cache) {
            for (idx_t i = 0; i <= cache_len; ++i) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }

        cache = new_cache;

        /* compute missing values */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 16) {
                cache[i] = init[i - 1];
            } else {
                /* recurrence computation, all arithmetic in seq_t */
                seq_t term = 2 * cache[i - 1];
                term -= cache[i - 2];
                term += 2 * cache[i - 4];
                term -= 4 * cache[i - 5];
                term += 2 * cache[i - 6];
                term += 2 * cache[i - 8];
                term -= 2 * cache[i - 9];
                cache[i] = term;
            }
        }

        cache_len = n;
    }

    return cache[n];
}
