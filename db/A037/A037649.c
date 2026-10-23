/*
 * A037649  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,1.
 * https://oeis.org/A037649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1)+a(n-3)-7*a(n-4); a(1)=3, a(2)=21, a(3)=148,
 *            a(4)=1039
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache so that calls with arbitrary order stay efficient */
    static seq_t *cache = NULL; /* cache[0] holds a(1) */
    static idx_t cache_max = 0; /* number of terms stored in cache */

    if (n < 1)
        return 0; /* undefined for n < 1 but return 0 for safety */

    /* enlarge cache if needed */
    if (n > cache_max) {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – abort, as the caller cannot recover */
            abort();
        }
        cache = tmp;

        /* compute new terms up to n */
        const seq_t seven = 7;
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            size_t idx = (size_t)(i - 1);

            if (i == 1)
                cache[idx] = 3;
            else if (i == 2)
                cache[idx] = 21;
            else if (i == 3)
                cache[idx] = 148;
            else if (i == 4)
                cache[idx] = 1039;
            else {
                /* a(i) = 7*a(i-1) + a(i-3) - 7*a(i-4) */
                cache[idx] = seven * cache[idx - 1]
                           + cache[idx - 3]
                           - seven * cache[idx - 4];
            }
        }
        cache_max = n;
    }

    return cache[(size_t)(n - 1)];
}
