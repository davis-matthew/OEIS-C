/*
 * A042421  Denominators of continued fraction convergents to sqrt(738).
 * https://oeis.org/A042421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 326*a(n-2) - a(n-4) with a(0)=1, a(1)=6, a(2)=325,
 *            a(3)=1956
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;      /* cache of computed values            */
    static size_t cap = 0;         /* current allocated capacity          */
    static idx_t last = (idx_t)-1; /* highest index that has been computed */

    /* Initialise the cache and fill the first four terms once.      */
    if (last == (idx_t)-1) {
        const size_t init = 4;
        buf = (seq_t *)malloc(init * sizeof(seq_t));
        if (!buf) return (seq_t)0; /* in a real program handle allocation failure properly */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)6;
        buf[2] = (seq_t)325;
        buf[3] = (seq_t)1956;
        cap  = init;
        last = 3;
    }

    /* Grow the cache if necessary.                                   */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* in a real program handle allocation failure properly */
        buf = tmp;
        cap = new_cap;
    }

    /* Compute any missing terms up to index n using the recurrence. */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* Indices i-2 and i-4 are always valid here because i ≥ 4. */
            buf[i] = (seq_t)326 * buf[i - 2] - buf[i - 4];
        }
        last = n;
    }

    return buf[n];
}
