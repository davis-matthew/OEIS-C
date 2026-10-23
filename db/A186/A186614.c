/*
 * A186614  Number of (n+5)X9 binary arrays with every 6X6 subblock commuting
 *          with each horizontal and vertical neighbor 6X6 subblock.
 * https://oeis.org/A186614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 52, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A186614: 
   Empirical linear recurrence (valid for n > 23):
   a(n) = a(n-1) + a(n-4) - a(n-5) + a(n-6) - a(n-7)
          - a(n-10) + a(n-11) + a(n-12) - a(n-16)
   Initial values:
   a(1)=654700, a(2)=25852, a(3)=30997, a(4)=88336, a(5)=439891, a(6)=1478651,
   a(7)=2676699, a(8)=3211547, a(9)=3276255, a(10)=3284387, a(11)=3291623,
   a(12)=3298867, a(13)=3306139, a(14)=3313675, a(15)=3324899, a(16)=3352139,
   a(17)=3475367, a(18)=4273524, a(19)=6521085, a(20)=9738958, a(21)=13025227,
   a(22)=16335644, a(23)=19749285, a(24)=23845099
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* return 0 for indices outside the defined range (n < 1) */
    if (n < 1)
        return (seq_t)0;

    /* static cache, lazily initialized */
    static seq_t *cache = NULL;   /* cache[0] unused, cache[1]..cache[24] are seed values */
    static idx_t cached = 0;       /* highest index with a computed value */

    /* Initialize cache with the first 24 terms */
    if (cached == 0) {
        cache = (seq_t *)malloc(25 * sizeof(seq_t));   /* indices 0..24 inclusive */
        if (!cache)
            return (seq_t)0;   /* allocation failure, return 0 */

        cache[0] = (seq_t)0;          /* unused */
        cache[1] = (seq_t)654700;
        cache[2] = (seq_t)25852;
        cache[3] = (seq_t)30997;
        cache[4] = (seq_t)88336;
        cache[5] = (seq_t)439891;
        cache[6] = (seq_t)1478651;
        cache[7] = (seq_t)2676699;
        cache[8] = (seq_t)3211547;
        cache[9] = (seq_t)3276255;
        cache[10] = (seq_t)3284387;
        cache[11] = (seq_t)3291623;
        cache[12] = (seq_t)3298867;
        cache[13] = (seq_t)3306139;
        cache[14] = (seq_t)3313675;
        cache[15] = (seq_t)3324899;
        cache[16] = (seq_t)3352139;
        cache[17] = (seq_t)3475367;
        cache[18] = (seq_t)4273524;
        cache[19] = (seq_t)6521085;
        cache[20] = (seq_t)9738958;
        cache[21] = (seq_t)13025227;
        cache[22] = (seq_t)16335644;
        cache[23] = (seq_t)19749285;
        cache[24] = (seq_t)23845099;
        cached = 24;
    }

    /* Expand cache if necessary and compute missing values using the recurrence */
    if (n > cached) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;   /* allocation failure */

        cache = new_cache;
        for (idx_t k = cached + 1; k <= n; ++k) {
            cache[k] =
                cache[k - 1] + cache[k - 4] - cache[k - 5] + cache[k - 6] - cache[k - 7]
                - cache[k - 10] + cache[k - 11] + cache[k - 12] - cache[k - 16];
        }
        cached = n;
    }

    return cache[n];
}
