/*
 * A041375  Denominators of continued fraction convergents to sqrt(202).
 * https://oeis.org/A041375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 6282*a(n-7) + a(n-14) a(0) = 1, a(1) = 4, a(2) = 5, a(3)
 *            = 14, a(4) = 33, a(5) = 47, a(6) = 221, a(7) = 6235, a(8) =
 *            25161, a(9) = 31396, a(10) = 87953, a(11) = 207302, a(12) =
 *            295255, a(13) = 1388322
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 19, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* capacity of the cache */
    static idx_t max_comp = (idx_t)-1;  /* highest index already computed */

    /* Ensure cache large enough for n plus the base indices 0..13 */
    size_t need = (n > 13 ? (size_t)n : 13) + 1;
    if (cap < need) {
        size_t newcap = cap > 0 ? cap * 2 : 16;
        while (newcap < need)
            newcap *= 2;
        /* C++ requires an explicit cast from void* to seq_t* */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failure: return zero */
        cache = tmp;
        cap = newcap;
    }

    /* initialise the first 14 terms only once */
    if (max_comp == (idx_t)-1) {
        cache[0]  = 1;
        cache[1]  = 4;
        cache[2]  = 5;
        cache[3]  = 14;
        cache[4]  = 33;
        cache[5]  = 47;
        cache[6]  = 221;
        cache[7]  = 6235;
        cache[8]  = 25161;
        cache[9]  = 31396;
        cache[10] = 87953;
        cache[11] = 207302;
        cache[12] = 295255;
        cache[13] = 1388322;
        max_comp = 13;
    }

    /* compute any terms beyond those already cached */
    if (n > max_comp) {
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            cache[i] = 6282 * cache[i - 7] + cache[i - 14];
        }
        max_comp = n;
    }

    return cache[n];
}
