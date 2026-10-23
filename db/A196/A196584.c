/*
 * A196584  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 3,2,0,1,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 6*a(n-2) + 4*a(n-3) - 3*a(n-4) - a(n-5) -
 *            2*a(n-6) - a(n-7) with initial values: a(1)=1, a(2)=7, a(3)=20,
 *            a(4)=65, a(5)=203, a(6)=648, a(7)=2057
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1; return 0 safely */

    static idx_t cache_size = 0;
    static seq_t *cache = NULL;
    const idx_t MAX_INIT = 7; /* number of known initial terms */

    /* Grow the cache if required */
    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed – safe return */
        cache = tmp;

        /* Compute or copy entries from cache_size to new_size-1 */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i < MAX_INIT) {
                /* Fill initial values directly */
                switch (i) {
                    case 0: cache[i] = (seq_t)1; break;
                    case 1: cache[i] = (seq_t)7; break;
                    case 2: cache[i] = (seq_t)20; break;
                    case 3: cache[i] = (seq_t)65; break;
                    case 4: cache[i] = (seq_t)203; break;
                    case 5: cache[i] = (seq_t)648; break;
                    case 6: cache[i] = (seq_t)2057; break;
                }
            } else {
                /* Apply the recurrence relation */
                seq_t val = cache[i-1];
                val += (seq_t)6 * cache[i-2];
                val += (seq_t)4 * cache[i-3];
                val -= (seq_t)3 * cache[i-4];
                val -= cache[i-5];
                val -= (seq_t)2 * cache[i-6];
                val -= cache[i-7];
                cache[i] = val;
            }
        }
        cache_size = new_size;
    }
    return cache[n-1];
}
