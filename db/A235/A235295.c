/*
 * A235295  Number of (n+1) X (5+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 7, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1)+17*a(n-2)-57*a(n-3)-86*a(n-4)
 *                 +372*a(n-5)+76*a(n-6)-972*a(n-7)+360*a(n-8)
 *                 +864*a(n-9)-576*a(n-10)   for n > 10
 * Initial values: a(1)=1888, a(2)=2556, a(3)=3536, a(4)=5784, a(5)=9568,
 *                 a(6)=18624, a(7)=35936, a(8)=79728, a(9)=172288, a(10)=420816
 */

#include <stdlib.h>          /* malloc, realloc */

seq_t a(idx_t n)
{
    /* memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0)
        return 0; /* undefined for n=0 */

    /* initial values for n=1..10 */
    static const seq_t init[10] = {
        (seq_t)1888,  (seq_t)2556,  (seq_t)3536,  (seq_t)5784,
        (seq_t)9568,  (seq_t)18624, (seq_t)35936, (seq_t)79728,
        (seq_t)172288,(seq_t)420816
    };

    if (cache_len < n) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return 0;                  /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_len; i < n; ++i) {
            if (i < 10) {
                cache[i] = init[i];
            } else {
                idx_t k = i + 1;        /* real index n = k */
                seq_t val = 0;
                val += (seq_t)3   * cache[k - 2];
                val += (seq_t)17  * cache[k - 3];
                val -= (seq_t)57  * cache[k - 4];
                val -= (seq_t)86  * cache[k - 5];
                val += (seq_t)372 * cache[k - 6];
                val += (seq_t)76  * cache[k - 7];
                val -= (seq_t)972 * cache[k - 8];
                val += (seq_t)360 * cache[k - 9];
                val += (seq_t)864 * cache[k -10];
                val -= (seq_t)576 * cache[k -11];
                cache[i] = val;
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
