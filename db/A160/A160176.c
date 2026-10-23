/*
 * A160176  Positive numbers y such that y^2 is of the form x^2+(x+617)^2 with
 *          integer x.
 * https://oeis.org/A160176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - a(n-6) for n > 6; Initial values: a(1)=533,
 *            a(2)=617, a(3)=733, a(4)=2465, a(5)=3085, a(6)=3865
 * Range:     u8 none, u16 n <= 9, u32 n <= 29, u64 n <= 66, u128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* 0‑based array of computed terms   */
    static idx_t  cache_cap = 0;      /* current capacity (in elements)    */
    static idx_t  cache_len = 0;      /* number of valid terms in cache   */

    if (n < 1) return 0;              /* undefined for n < 1, return 0   */

    /* make sure the cache holds at least n terms */
    if (cache_len < n) {
        idx_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 8;          /* initial capacity */
        while (new_cap < n) new_cap *= 2;       /* grow by factor 2 */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);          /* allocation error */
        cache = tmp;
        cache_cap = new_cap;

        /* fill new entries */
        for (idx_t i = cache_len; i < n; ++i) {
            if (i < 6) {  /* initial values */
                switch (i) {
                case 0: cache[i] = (seq_t)533;   break;
                case 1: cache[i] = (seq_t)617;   break;
                case 2: cache[i] = (seq_t)733;   break;
                case 3: cache[i] = (seq_t)2465;  break;
                case 4: cache[i] = (seq_t)3085;  break;
                case 5: cache[i] = (seq_t)3865;  break;
                }
            } else {  /* recurrence a(n) = 6*a(n-3) - a(n-6) */
                cache[i] = 6 * cache[i - 3] - cache[i - 6];
            }
        }
        cache_len = n;
    }

    return cache[n - 1];
}
