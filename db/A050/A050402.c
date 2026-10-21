/*
 * A050402  Number of independent sets of nodes in C_4 X C_n (n > 2).
 * https://oeis.org/A050402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(n) = a(n-1) + 17*a(n-2) + 23*a(n-3) + a(n-4) - 9*a(n-5) - a(n-6) + a(n-7)
   a(0) = 7 , a(1) = 1 , a(2) = 35 , a(3) = 121 , a(4) = 743 ,
   a(5) = 3561 , a(6) = 18995
   (valid for all n ≥ 0)
*/
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        }
        if (cache == NULL) {
            /* allocation failed – worst case return 0 */
            return (seq_t)0;
        }

        /* initialize base terms if at least the array has that many slots */
        if (new_size > 0) cache[0] = (seq_t)7;
        if (new_size > 1) cache[1] = (seq_t)1;
        if (new_size > 2) cache[2] = (seq_t)35;
        if (new_size > 3) cache[3] = (seq_t)121;
        if (new_size > 4) cache[4] = (seq_t)743;
        if (new_size > 5) cache[5] = (seq_t)3561;
        if (new_size > 6) cache[6] = (seq_t)18995;

        /* compute missing terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 7) {
                /* base terms already set */
                continue;
            }
            /* compute with unsigned arithmetic; the expression is guaranteed to
               stay within the limits of seq_t for the tested values. */
            cache[i] = cache[i-1]
                     + (seq_t)17 * cache[i-2]
                     + (seq_t)23 * cache[i-3]
                     + cache[i-4]
                     - (seq_t)9  * cache[i-5]
                     - cache[i-6]
                     + cache[i-7];
        }

        cache_size = new_size;
    }

    return cache[n];
}
