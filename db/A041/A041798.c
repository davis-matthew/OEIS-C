/*
 * A041798  Numerators of continued fraction convergents to sqrt(420).
 * https://oeis.org/A041798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041798: a(n) = 82*a(n-2) - a(n-4)
   with initial values a(0)=20, a(1)=41, a(2)=1660, a(3)=3361       */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t  csize   = 0;
    /*  Grow the cache if the requested index is out of bounds   */
    if (n >= csize) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache)
            return 0;                     /* allocation failure */

        if (cache)                        /* copy existing values   */
            memcpy(new_cache, cache, csize * sizeof(seq_t));

        /*  Initialise new entries, using the given recurrence   */
        for (idx_t i = csize; i < new_size; ++i) {
            if (i == 0)
                new_cache[0] = (seq_t)20;
            else if (i == 1)
                new_cache[1] = (seq_t)41;
            else if (i == 2)
                new_cache[2] = (seq_t)1660;
            else if (i == 3)
                new_cache[3] = (seq_t)3361;
            else
                new_cache[i] = (seq_t)82 * new_cache[i - 2] - new_cache[i - 4];
        }

        free(cache);                       /* release old cache     */
        cache   = new_cache;
        csize   = new_size;
    }
    return cache[n];
}
