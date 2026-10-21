/*
 * A203900  a(n)=f(a(n-1)+1,a(n-2),a(n-3)+1), where f(x,y,z)=yz+zx+xy and
 *          (a(1),a(2),a(3))=(0,0,1).
 * https://oeis.org/A203900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(a(n-1)+1,a(n-2),a(n-3)+1) where f(x,y,z)=yz+zx+xy
 *            Initial values: a(1)=0, a(2)=0, a(3)=1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* current allocated size (index + 1) */

    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1, return 0 for safety */

    if (cache_len <= n) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;          /* we use 1‑based indexing; index 0 unused */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)      /* allocation failure – unlikely in test harness */
            return (seq_t)0;
        cache = tmp;
        cache_len = new_len;

        idx_t start = old_len;
        if (start == 0)
            start = 1;    /* if cache was empty, start from 1 */

        for (idx_t i = start; i <= n; ++i) {
            if (i == 1)
                cache[i] = (seq_t)0;
            else if (i == 2)
                cache[i] = (seq_t)0;
            else if (i == 3)
                cache[i] = (seq_t)1;
            else {
                seq_t x = cache[i-1] + (seq_t)1;
                seq_t y = cache[i-2];
                seq_t z = cache[i-3] + (seq_t)1;
                cache[i] = y*z + z*x + x*y;
            }
        }
    }
    return cache[n];
}
