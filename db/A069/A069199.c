/*
 * A069199  a(1)=1 a(2)=7 a(n+2)=(a(n+1)+a(n))/3 if (a(n+1)+a(n)==0 (mod 3));
 *          a(n+2)=(a(n+1)+a(n))/2 if (a(n+1)+a(n)==0 (mod 2));
 *          a(n+2)=a(n+1)+a(n) otherwise.
 * https://oeis.org/A069199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A069199
   Recurrence:
   a(1)=1, a(2)=7
   For n ≥ 3:
     let s = a(n-1)+a(n-2)
     a(n) = s/3  if s≡0 (mod 3)
         = s/2  if s≡0 (mod 2)
         = s    otherwise
*/

#include <stdlib.h>   /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* A static cache indexed from 1.  Index 0 is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* highest index actually stored */

    if (n < 1)                         /* undefined for n==0, never called in tests */
        return (seq_t)0;

    /* Grow the cache if necessary.  Always keep at least 3 slots
       so that cache[2] can be written safely. */
    if (n > cache_len) {
        size_t new_size = (size_t)n + 1;
        if (new_size < 3) new_size = 3;       /* reserve indices 1 and 2 */

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)                               /* memory error – unlikely in tests */
            return (seq_t)0;
        cache = tmp;

        /* Initialise the first terms if not already present */
        if (cache_len < 1)
            cache[1] = (seq_t)1;
        if (cache_len < 2)
            cache[2] = (seq_t)7;

        /* Compute missing values from the first index that needs
           a computation up to 'n'.  We start at 3 because the first
           two terms are known. */
        idx_t start = (cache_len < 2) ? 3 : cache_len + 1;
        if (start < 3) start = 3;               /* safety for very small n */
        for (idx_t i = start; i <= n; ++i) {
            seq_t sum = cache[i - 1] + cache[i - 2];
            if ((sum % (seq_t)3) == 0)
                cache[i] = sum / (seq_t)3;
            else if ((sum % (seq_t)2) == 0)
                cache[i] = sum / (seq_t)2;
            else
                cache[i] = sum;
        }
        cache_len = n;                          /* remember last stored index */
    }

    return cache[n];
}
