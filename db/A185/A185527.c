/*
 * A185527  Number of (n+2)X4 binary arrays with each 3X3 subblock
 *          nonsingular.
 * https://oeis.org/A185527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A185527:
   a(n) = 3*a(n-1)+8*a(n-2)+77*a(n-3)-79*a(n-4)-180*a(n-5)-1228*a(n-6)+48*a(n-7)+232*a(n-8)+4208*a(n-9)-1184*a(n-10)-1664*a(n-11)-6144*a(n-12)
   for n>12.
   Initial values (indices from 1):
   1: 726
   2: 4362
   3: 25854
   4: 150234
   5: 886926
   6: 5221098
   7: 30631134
   8: 180225834
   9: 1059790686
   10: 6228272058
   11: 36622083390
   12: 215312392794
*/
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;            /* cache[0] <-- a(1) */
    static size_t cache_size = 0;
    static size_t max_computed = 0;         /* largest n computed (1‑based) */

    /* Initial values for a(1)…a(12) */
    static const seq_t init[12] = {
        726, 4362, 25854, 150234, 886926, 5221098,
        30631134, 180225834, 1059790686, 6228272058,
        36622083390, 215312392794
    };

    /* Allocate or enlarge cache if necessary */
    if (n == 0) return 0; /* undefined; safety */
    if (cache == NULL) {
        cache_size = (n > 12) ? n : 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        for (size_t i = 0; i < 12; ++i) cache[i] = init[i];
        max_computed = 12;
    } else if ((size_t)n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
        cache_size = n;
        /* no need to initialize new entries yet */
    }

    /* Compute any missing values up to n */
    if (n > max_computed) {
        for (size_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = 0;
            /* a(i) depends on a(i-1)...a(i-12) which are stored at cache[i-2] ... cache[i-13] */
            val += 3 * cache[i - 2];
            val += 8 * cache[i - 3];
            val += 77 * cache[i - 4];
            val -= 79 * cache[i - 5];
            val -= 180 * cache[i - 6];
            val -= 1228 * cache[i - 7];
            val += 48 * cache[i - 8];
            val += 232 * cache[i - 9];
            val += 4208 * cache[i - 10];
            val -= 1184 * cache[i - 11];
            val -= 1664 * cache[i - 12];
            val -= 6144 * cache[i - 13];
            cache[i - 1] = val;
        }
        max_computed = n;
    }

    return cache[n - 1];
}
