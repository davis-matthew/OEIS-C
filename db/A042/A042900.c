/*
 * A042900  Numerators of continued fraction convergents to sqrt(982).
 * https://oeis.org/A042900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 17674*a(n-6) - a(n-12) for n ≥ 12 Initial terms: a(0)=31,
 *            a(1)=63, a(2)=94, a(3)=2883, a(4)=2977, a(5)=8837, a(6)=550871,
 *            a(7)=1110579, a(8)=1661450, a(9)=50954079, a(10)=52615529,
 *            a(11)=156185137
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t next   = 0;

    /* Return cached value when available */
    if (n < next && cache != NULL)
        return cache[n];

    /* Allocate more space if necessary */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < needed)
            newcap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* Base values (0 ≤ n ≤ 11) */
    static const idx_t base_val[12] = {
        31, 63, 94, 2883, 2977, 8837,
        550871, 1110579, 1661450, 50954079,
        52615529, 156185137
    };

    /* Initialize base terms if necessary */
    if (next < 12) {
        idx_t upto = (n < 12) ? n : 11;
        for (idx_t i = next; i <= upto; ++i)
            cache[i] = (seq_t)base_val[i];
        next = upto + 1;                 /* next index to compute */
    }

    /* Compute remaining terms using the recurrence */
    for (idx_t i = next; i <= n; ++i) {
        cache[i] = (seq_t)17674 * cache[i - 6] - cache[i - 12];
    }
    next = n + 1;                         /* update cache boundary */

    return cache[n];
}
