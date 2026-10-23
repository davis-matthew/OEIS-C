/*
 * A195972  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,0,1,3,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A195972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) - a(n-4) - 2*a(n-5) - 2*a(n-6) - a(n-7)
 *            initial values: a(1)=3, a(2)=9, a(3)=14, a(4)=23, a(5)=43,
 *            a(6)=78, a(7)=139
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 72, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */
    static seq_t *cache = NULL;              /* cache of computed values  */
    static idx_t cache_len = 0;               /* current cache size (index+1) */

    if (cache_len <= n) {                    /* grow cache if needed */
        idx_t new_len = n + 1;                /* allocate up to index n */
        seq_t *tmp = (seq_t *)realloc(cache,
                                      new_len * sizeof(seq_t));
        if (!tmp) {                           /* allocation failed – abort */
            /* In a real library we might handle this differently,
               but for this solution we simply return 0. */
            return 0;
        }
        cache = tmp;

        /* Initialise new entries from old_len up to n (inclusive) */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i <= 7) {                     /* base values */
                switch (i) {
                    case 1: cache[1] = (seq_t)3; break;
                    case 2: cache[2] = (seq_t)9; break;
                    case 3: cache[3] = (seq_t)14; break;
                    case 4: cache[4] = (seq_t)23; break;
                    case 5: cache[5] = (seq_t)43; break;
                    case 6: cache[6] = (seq_t)78; break;
                    case 7: cache[7] = (seq_t)139; break;
                    default: cache[i] = 0;      /* shouldn't happen */
                }
            } else {                           /* recurrence for i>=8 */
                seq_t t1 = 2 * cache[i-1] + cache[i-3];
                seq_t t2 = cache[i-4]
                         + 2 * cache[i-5]
                         + 2 * cache[i-6]
                         + cache[i-7];
                cache[i] = t1 - t2;             /* t1 >= t2 for all i>=8 */
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
