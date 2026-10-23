/*
 * A133448  a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), with initial values
 *          2,5,13,40.
 * https://oeis.org/A133448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(1)=5, a(2)=13, a(3)=40; a(n)=3*a(n-1)-a(n-3)+3*a(n-4)
 *            for n>=4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t sz    = 0;            /* number of stored terms (index+1) */

    /* Ensure the cache is large enough to contain a(n).  We always keep at
       least four slots to hold the initial values. */
    idx_t need = (n >= 3) ? n + 1 : 4;
    if (need > sz) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;

        /* Initialise the first four terms (safe to reassign). */
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)13;
        cache[3] = (seq_t)40;

        /* Compute any new terms beyond what was already computed. */
        for (idx_t i = (sz > 3) ? sz : 4; i <= n; ++i) {
            cache[i] = 3 * cache[i-1] - cache[i-3] + 3 * cache[i-4];
        }

        sz = need;
    }

    return cache[n];
}
