/*
 * A099672  Partial sums of repdigits of A002279.
 * https://oeis.org/A099672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (recurrence): a(1)=5, a(2)=60, a(3)=615,
   a(n)=12*a(n-1)-21*a(n-2)+10*a(n-3)  for n>3
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = 0;
    static idx_t cache_cap = 0;

    if (n < 1) return 0;                 /* undefined for n<1, but guard */

    /* enlarge cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = n;                /* we want to store a[1..n] */

#ifdef __cplusplus
        /* C++: use operator new[] and delete[] for arbitrary–precision types */
        seq_t *tmp = new seq_t[new_cap + 1];
        for (idx_t i = 1; i <= cache_cap; ++i) tmp[i] = cache[i];
        delete[] cache;
#else
        /* C: use realloc for POD types */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(new_cap + 1) * sizeof(seq_t));
        if (!tmp) { abort(); } /* allocation failed */
        cache = tmp;
#endif

        cache = tmp;

        /* compute new entries */
        for (idx_t i = cache_cap + 1; i <= new_cap; ++i) {
            if (i == 1) cache[1] = (seq_t)5;
            else if (i == 2) cache[2] = (seq_t)60;
            else if (i == 3) cache[3] = (seq_t)615;
            else
                cache[i] = (seq_t)((seq_t)12 * cache[i-1]
                                   - (seq_t)21 * cache[i-2]
                                   + (seq_t)10 * cache[i-3]);
        }
        cache_cap = new_cap;
    }

    return cache[n];
}
