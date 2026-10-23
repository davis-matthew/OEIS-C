/*
 * A196690  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,1,3,0,4
 *          for x=0,1,2,3,4.
 * https://oeis.org/A196690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=2, a(2)=5, a(3)=11, a(4)=34, a(5)=101; 
 * for n>5: a(n) = 2*a(n-1) + 3*a(n-2) - a(n-4) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n < 1)
        return 0;          /* undefined for n<1 */

    static seq_t *cache = NULL;
    static idx_t cache_n = 0;   /* highest index currently cached */

    if (n > cache_n) {
        size_t new_cap = (size_t)n + 1;   /* array size needed for indices 0..n */
        seq_t *tmp;
        if (cache == NULL)
            tmp = (seq_t*)malloc(new_cap * sizeof(seq_t));
        else
            tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* allocation failed */
            return 0;
        cache = tmp;

        /* compute values from cache_n+1 up to n */
        for (idx_t i = cache_n + 1; i <= n; ++i) {
            if (i == 1)
                cache[i] = 2;
            else if (i == 2)
                cache[i] = 5;
            else if (i == 3)
                cache[i] = 11;
            else if (i == 4)
                cache[i] = 34;
            else if (i == 5)
                cache[i] = 101;
            else
                cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2] - cache[i - 4];
        }
        cache_n = n;
    }

    return cache[n];
}
