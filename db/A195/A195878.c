/*
 * A195878  y-values in the solution to 7*x^2-6 = y^2.
 * https://oeis.org/A195878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=13, a(3)=29, a(4)=209,
   a(n) = 16*a(n-2) - a(n-4) for n >= 5. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* static cache of previously computed values */
    static seq_t *cache = NULL;   /* array indexed by n; cache[0] unused */
    static idx_t cache_size = 0;  /* highest n that has been computed */

    /* initially allocate space for indices 0..4 */
    if (cache == NULL) {
        cache = (seq_t *)malloc((size_t)5 * sizeof(seq_t));
        if (cache == NULL)          /* allocation failure */
            return (seq_t)0;
        cache[0] = (seq_t)0;        /* unused */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)29;
        cache[4] = (seq_t)209;
        cache_size = 4;
    }

    /* extend cache if needed */
    if (n > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL)           /* allocation failure */
            return (seq_t)0;
        cache = tmp;
        /* compute new values using the recurrence */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            /* a(i) = 16*a(i-2) - a(i-4) */
            cache[i] = (cache[i - 2] << 4) - cache[i - 4];
        }
        cache_size = n;
    }

    return cache[n];
}
