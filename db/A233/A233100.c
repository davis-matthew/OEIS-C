/*
 * A233100  Number of 3 X n 0..3 arrays with no element x(i,j) adjacent to
 *          value 3-x(i,j) horizontally, diagonally or antidiagonally, top
 *          left element zero, and 1 appearing before 2 in row major order.
 * https://oeis.org/A233100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 5*a(n-2) - 47*a(n-3) + 52*a(n-4) - 12*a(n-5)
 *            for n > 6, with initial values a(1)=10, a(2)=74, a(3)=941,
 *            a(4)=11486, a(5)=141566, a(6)=1742447
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static idx_t   max_cache = 0;   /* largest n for which cache is valid   */
static seq_t *cache = NULL;     /* 1‑based array of pre‑computed terms   */

seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* undefined for n<1, harmless   */

    /* grow cache up to position n (inclusive) */
    if (n > max_cache) {
        size_t new_cap = (size_t)(n + 1);    /* include index 0 unused       */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* allocation failed            */
        cache = tmp;

        /* initialise initial values if needed */
        if (max_cache < 1 && n >= 1) cache[1] = (seq_t)10;
        if (max_cache < 2 && n >= 2) cache[2] = (seq_t)74;
        if (max_cache < 3 && n >= 3) cache[3] = (seq_t)941;
        if (max_cache < 4 && n >= 4) cache[4] = (seq_t)11486;
        if (max_cache < 5 && n >= 5) cache[5] = (seq_t)141566;
        if (max_cache < 6 && n >= 6) cache[6] = (seq_t)1742447;

        /* compute further terms using the recurrence */
        for (idx_t i = (max_cache > 6 ? max_cache + 1 : 7); i <= n; ++i) {
            cache[i] = (seq_t)13 * cache[i-1]
                     - (seq_t)5  * cache[i-2]
                     - (seq_t)47 * cache[i-3]
                     + (seq_t)52 * cache[i-4]
                     - (seq_t)12 * cache[i-5];
        }
        max_cache = n;
    }

    return cache[n];
}
