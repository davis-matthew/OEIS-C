/*
 * A038504  Sum of every 4th entry of row n in Pascal's triangle, starting at
 *          "n choose 1".
 * https://oeis.org/A038504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 4*a(n-3) with a(0) = 0, a(1) = 1,
 *            a(2) = 2, a(3) = 3, for all n ≥ 0
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;        /* current number of cached entries */

    /* enlarge cache if necessary */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        /* realloc returns void* in C++, cast to seq_t* */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failed */
        cache = tmp;

        /* compute new entries */
        for (idx_t i = cap; i <= n; ++i) {
            if (i == 0)
                cache[i] = (seq_t)0;
            else if (i == 1)
                cache[i] = (seq_t)1;
            else if (i == 2)
                cache[i] = (seq_t)2;
            else if (i == 3)
                cache[i] = (seq_t)3;
            else {
                seq_t t1 = (seq_t)4 * cache[i-1];
                seq_t t2 = (seq_t)6 * cache[i-2];
                seq_t t3 = (seq_t)4 * cache[i-3];
                cache[i] = t1 - t2 + t3;
            }
        }
        cap = new_cap;
    }
    return cache[n];
}
