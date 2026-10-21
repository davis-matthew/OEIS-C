/*
 * A235179  Number of (n+1) X (1+1) 0..5 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-2) - 65*a(n-4) + 14*a(n-6) Initial values:
 *            a(1)=72, a(2)=212, a(3)=576, a(4)=1696, a(5)=4656, a(6)=13736
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] corresponds to a(1) */
    static idx_t size  = 0;          /* number of cached terms */

    if (n < 1)                /* undefined for 0, return 0 */
        return 0;

    /* enlarge cache if necessary */
    if (n > size) {
        idx_t new_size = n;
#if defined(__cplusplus)
        /* C++: allocate new storage and copy old data */
        seq_t *newcache = new seq_t[new_size];
        for (idx_t i = 0; i < size; ++i)
            newcache[i] = cache[i];
        delete[] cache;
#else
        /* C: realloc preserves existing data */
        seq_t *newcache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!newcache)
            abort();
#endif
        cache = newcache;
        size  = size; /* keep old size until we fill new entries */
        /* fill new terms from old size to new_size-1 */
        for (idx_t i = size; i < new_size; ++i) {
            if (i < 6) { /* base values a(1)..a(6) at indices 0..5 */
                switch (i) {
                    case 0: cache[i] = (seq_t)72;   break;
                    case 1: cache[i] = (seq_t)212;  break;
                    case 2: cache[i] = (seq_t)576;  break;
                    case 3: cache[i] = (seq_t)1696; break;
                    case 4: cache[i] = (seq_t)4656; break;
                    case 5: cache[i] = (seq_t)13736;break;
                }
            } else { /* recurrence for i >= 6 (i.e., n >= 7) */
                cache[i] = 16 * cache[i - 2]
                           - 65 * cache[i - 4]
                           + 14 * cache[i - 6];
            }
        }
        size = new_size; /* now all entries up to n are computed */
    }

    /* cache[n-1] holds a(n) */
    return cache[n - 1];
}
