/*
 * A222331  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..5 array extended with zeros and
 *          convolved with 1,1.
 * https://oeis.org/A222331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-3) + a(n-5) + a(n-7) + a(n-9) + a(n-11),
   initial values: a(1)=2, a(2)=3, a(3)=4, a(4)=7, a(5)=11, a(6)=18,
   a(7)=29, a(8)=47, a(9)=76, a(10)=123, a(11)=199 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    static idx_t known_max = 0;   /* largest n for which cache[n] is defined */

    /* ensure cache large enough to hold index n */
    if ((idx_t)(cache_capacity) <= n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_capacity = (idx_t)new_cap;
    }

    /* initialize base values once (indices 1 … 11) */
    if (known_max < 11) {
        /* ensure capacity for first 11 indices */
        if (cache_capacity < 12) {
            size_t new_cap = 12;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            cache = tmp;
            cache_capacity = (idx_t)new_cap;
        }
        cache[1] = 2;    cache[2] = 3;    cache[3] = 4;
        cache[4] = 7;    cache[5] = 11;   cache[6] = 18;
        cache[7] = 29;   cache[8] = 47;   cache[9] = 76;
        cache[10] = 123; cache[11] = 199;
        known_max = 11;
    }

    /* compute values up to n if needed */
    if (n > known_max) {
        for (idx_t i = known_max + 1; i <= n; ++i) {
            seq_t sum = 0;
            sum += cache[i - 1];
            sum += cache[i - 3];
            sum += cache[i - 5];
            sum += cache[i - 7];
            sum += cache[i - 9];
            sum += cache[i - 11];
            cache[i] = sum;
        }
        known_max = n;
    }

    return cache[n];
}
