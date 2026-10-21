/*
 * A055518  a_{k+1} = 6*a_k + 11*a_{k-1} - 19*a_{k-2} - 4*a_{k-3} + a_{k-4},
 *          a_1=1, a_2=2, a_3=19, a_4=118, a_5=875.
 * https://oeis.org/A055518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(k+1) = 6*a(k) + 11*a(k-1) - 19*a(k-2) - 4*a(k-3) + a(k-4)
 *            Initial terms: a(1)=1, a(2)=2, a(3)=19, a(4)=118, a(5)=875 (The
 *            function takes any 1‑based index n and returns a(n).)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* allocated size (max index that may be stored) */
    static idx_t  len   = 0;   /* highest index for which cache[i] is valid */

    /* make sure the cache can hold at least indices up to n and up to 5 */
    idx_t need = n;
    if (need < 5) need = 5;
    if (cap < need) {
        idx_t newcap = (cap == 0) ? 8 : cap * 2;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (tmp == NULL) {
            /* allocation failure – terminate the program */
            exit(EXIT_FAILURE);
        }
        cache = tmp;
        cap   = newcap;
    }

    /* initialise the first five terms if this is the first call */
    if (len < 5) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)19;
        cache[4] = (seq_t)118;
        cache[5] = (seq_t)875;
        len = 5;
    }

    /* for n <= 5 just return the stored value */
    if (n <= 5) {
        return cache[n];
    }

    /* compute any missing terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        /* the recurrence is well defined for i >= 6 */
        seq_t val = (seq_t)6   * cache[i - 1]
                  + (seq_t)11  * cache[i - 2]
                  - (seq_t)19  * cache[i - 3]
                  - (seq_t)4   * cache[i - 4]
                  +          cache[i - 5];
        cache[i] = val;
    }
    len = n;

    return cache[n];
}
