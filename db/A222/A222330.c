/*
 * A222330  Number of binary arrays indicating the locations of trailing edge
 *          maxima of a random length-n 0..4 array extended with zeros and
 *          convolved with 1,1.
 * https://oeis.org/A222330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) + a(n-7) + a(n-9); Initial
 *            values: a(1)=2, a(2)=3, a(3)=4, a(4)=7, a(5)=11, a(6)=18,
 *            a(7)=29, a(8)=47, a(9)=76
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* the sequence is defined only for n >= 1 */
    if (n == 0)
        return (seq_t)0;

    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array indexed from 1 */
    static size_t cache_len  = 0;      /* highest index computed */
    static size_t cache_cap  = 0;      /* current allocated capacity */

    /* ensure capacity for indices up to n (including 0) */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed)
            new_cap <<= 1;           /* grow by factor of 2 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                /* out of memory */
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first nine terms once */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;          /* unused sentinel */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)7;
        cache[5] = (seq_t)11;
        cache[6] = (seq_t)18;
        cache[7] = (seq_t)29;
        cache[8] = (seq_t)47;
        cache[9] = (seq_t)76;
        cache_len = 9;
    }

    /* compute terms up to requested n */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* all indices i-1, i-3, i-5, i-7, i-9 are ≥ 1 for i ≥ 10 */
            seq_t sum = cache[i - 1];
            sum += cache[i - 3];
            sum += cache[i - 5];
            sum += cache[i - 7];
            sum += cache[i - 9];
            cache[i] = sum;
        }
        cache_len = (size_t)n;
    }

    return cache[n];
}
