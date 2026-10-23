/*
 * A173434  a(n) = (A000045(n)-A173432(n))/2.
 * https://oeis.org/A173434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-6);
   initial values: a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=2, a(6)=4 */

#include <stdlib.h>

/* Compute a(n) for n >= 1.  The function is memoised; it can be called
   in any order.  The result fits in seq_t; no larger intermediate value
   is required. */
seq_t a(idx_t n)
{
    /* static cache remembers computed terms */
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated size of cache   */
    static idx_t  last   = 0;   /* largest index computed    */

    /* enlarge cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        if (newcap < 8) newcap = 8;          /* start small but accommodate the first few terms */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof *cache);
        if (!tmp) abort();                  /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            if (i <= 6) {                    /* seed values */
                switch ((int)i) {
                case 1: cache[i] = 0; break;
                case 2: cache[i] = 0; break;
                case 3: cache[i] = 0; break;
                case 4: cache[i] = 1; break;
                case 5: cache[i] = 2; break;
                case 6: cache[i] = 4; break;
                }
            } else {                          /* recurrence */
                seq_t t = 2 * cache[i - 1];
                t -= 2 * cache[i - 3];
                t += 2 * cache[i - 4];
                t -= cache[i - 6];
                cache[i] = t;
            }
        }
        last = n;
    }

    return cache[n];
}
