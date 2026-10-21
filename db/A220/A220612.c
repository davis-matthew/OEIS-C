/*
 * A220612  Number of ways to reciprocally link elements of an nX6 array
 *          either to themselves or to exactly two horizontal and vertical
 *          neighbors, without consecutive collinear links.
 * https://oeis.org/A220612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 23*a(n-2) - 40*a(n-3) - 160*a(n-4) * +
 *            190*a(n-5) + 441*a(n-6) - 369*a(n-7) - 513*a(n-8) * + 278*a(n-9)
 *            + 240*a(n-10) - 64*a(n-11) - 39*a(n-12) * + 3*a(n-13) + a(n-14)
 *            * Initial values: a(1)=1, a(2)=13, a(3)=43, a(4)=281, a(5)=1277,
 *            * a(6)=7041, a(7)=34981, a(8)=183920, a(9)=937...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n < 1, but harmless */

    /* static cache: cache[i] holds a(i+1) for i>=0 */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;        /* current number of stored terms */

    /* expand cache if necessary */
    if ((size_t)n > cache_size)
    {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;             /* allocation failure */
        cache = tmp;

        /* initial 14 terms */
        static const seq_t init[14] = {
            (seq_t)1,
            (seq_t)13,
            (seq_t)43,
            (seq_t)281,
            (seq_t)1277,
            (seq_t)7041,
            (seq_t)34981,
            (seq_t)183920,
            (seq_t)937663,
            (seq_t)4861869,
            (seq_t)24977087,
            (seq_t)128971605,
            (seq_t)664092737,
            (seq_t)3424806465
        };

        for (size_t i = cache_size; i < new_size; ++i)
        {
            if (i < 14)
            {
                cache[i] = init[i];
            }
            else
            {
                /* i is 0-based; a(i+1) depends on a(i) ... a(i-13) */
                seq_t val = (seq_t)0;
                val += (seq_t)3   * cache[i - 1];
                val += (seq_t)23  * cache[i - 2];
                val -= (seq_t)40  * cache[i - 3];
                val -= (seq_t)160 * cache[i - 4];
                val += (seq_t)190 * cache[i - 5];
                val += (seq_t)441 * cache[i - 6];
                val -= (seq_t)369 * cache[i - 7];
                val -= (seq_t)513 * cache[i - 8];
                val += (seq_t)278 * cache[i - 9];
                val += (seq_t)240 * cache[i -10];
                val -= (seq_t)64  * cache[i -11];
                val -= (seq_t)39  * cache[i -12];
                val += (seq_t)3   * cache[i -13];
                val +=             cache[i -14];
                cache[i] = val;
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n - 1];
}
