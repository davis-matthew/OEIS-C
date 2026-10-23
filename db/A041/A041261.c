/*
 * A041261  Denominators of continued fraction convergents to sqrt(142).
 * https://oeis.org/A041261
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 286*a(n-4) - a(n-8) for n >= 8 Initial terms: a(0)=1,
 *            a(1)=1, a(2)=11, a(3)=12, a(4)=275, a(5)=287, a(6)=3145,
 *            a(7)=3432 All other terms are computed by the recurrence
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;           /* array of computed values   */
    static size_t cap   = 0;              /* allocated array size       */
    static idx_t  max   = 0;              /* highest index computed     */
    static bool   init   = false;         /* whether initial values set */

    /* Ensure storage for at least max(n,7) + 1 elements */
    idx_t need = (n >= 7 ? n : 7);
    if (!cache || cap <= need) {
        void *tmp = realloc(cache, (need + 1) * sizeof(seq_t));
        if (!tmp) abort();             /* allocation failed */
        cache = (seq_t *)tmp;
        cap   = need + 1;
    }

    /* Initialise the first eight terms once */
    if (!init) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)275;
        cache[5] = (seq_t)287;
        cache[6] = (seq_t)3145;
        cache[7] = (seq_t)3432;
        max   = 7;
        init  = true;
    }

    /* Return stored value if already computed */
    if (n <= max) return cache[n];

    /* Compute subsequent terms up to desired n */
    for (idx_t i = max + 1; i <= n; ++i) {
        cache[i] = (seq_t)286 * cache[i - 4] - cache[i - 8];
    }

    max = n;
    return cache[n];
}
