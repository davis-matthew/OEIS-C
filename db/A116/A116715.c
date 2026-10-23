/*
 * A116715  Number of permutations of length n which avoid the patterns 312,
 *          2341, 4321.
 * https://oeis.org/A116715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 2*a(n-3) + 3*a(n-4) + a(n-5) Initial
 *            values: a(1)=1, a(2)=2, a(3)=5, a(4)=12, a(5)=25
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 57, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] holds a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 1)
        return (seq_t)0;

    if (n > cache_size) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)   /* allocation failure: return 0 */
            return (seq_t)0;
        cache = tmp;

        for (idx_t i = cache_size + 1; i <= new_size; ++i) {
            seq_t val;
            if (i == 1)
                val = (seq_t)1;
            else if (i == 2)
                val = (seq_t)2;
            else if (i == 3)
                val = (seq_t)5;
            else if (i == 4)
                val = (seq_t)12;
            else if (i == 5)
                val = (seq_t)25;
            else
                val = cache[i-2]   /* a(i-1) */
                    + cache[i-3]   /* a(i-2) */
                    + ((seq_t)2) * cache[i-4]   /* 2*a(i-3) */
                    + ((seq_t)3) * cache[i-5]   /* 3*a(i-4) */
                    + cache[i-6];                 /* a(i-5) */
            cache[i-1] = val;
        }
        cache_size = new_size;
    }

    return cache[n-1];
}
