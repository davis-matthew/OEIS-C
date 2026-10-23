/*
 * A117547  Numbers k such that 2*k^2+1 is a perfect power.
 * https://oeis.org/A117547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117547
   Formula: a(1)=0, a(2)=2, a(3)=11, a(4)=12, a(5)=70,
           a(n)=6*a(n-1)-a(n-2)  for n>5
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* --- initial known values ---------------------------------------- */
    static const seq_t first[6] = {0, 0, 2, 11, 12, 70};

    if (n <= 5)
        return first[n];

    /* --- static cache for previously computed terms ------------------ */
    static seq_t *cache   = NULL; /* pointers to cache[0 .. cache_n] */
    static idx_t  cache_n = 0;    /* largest index stored */

    /* --- initialise cache on first use ------------------------------ */
    if (cache == NULL) {
        size_t init_size = 6;                /* indices 0..5 used */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        /* Omitted: if (cache==NULL) return ??? (assume enough memory)   */
        for (idx_t i = 1; i <= 5; ++i)
            cache[i] = first[i];
        cache_n = 5;
    }

    /* --- grow cache if we are asked for a larger index ------------- */
    if (n > cache_n) {
        size_t new_len = (size_t)(n + 1);    /* allocate indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* Omitted: if (tmp==NULL) return ??? (assume enough memory)   */
        cache = tmp;
        for (idx_t j = cache_n + 1; j <= n; ++j) {
            /* recurrence:  a(j) = 6*a(j-1) - a(j-2)  */
            cache[j] = (seq_t)6 * cache[j - 1] - cache[j - 2];
        }
        cache_n = n;
    }

    return cache[n];
}
