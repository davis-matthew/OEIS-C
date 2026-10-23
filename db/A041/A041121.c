/*
 * A041121  Denominators of continued fraction convergents to sqrt(69).
 * https://oeis.org/A041121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041121: Denominators of continued fraction convergents to sqrt(69)
   Recurrence:
     a(n) = 15550*a(n-8) - a(n-16)   for n ≥ 16
   Initial values a(0) … a(15) are those listed in the OEIS entry. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* coefficient of the recurrence, converted to seq_t */
    const seq_t coeff = (seq_t)15550;

    /* initial 16 terms */
    const seq_t init[16] = {
        1, 3, 10, 13, 62, 75, 287, 936,
        15263, 46725, 155438, 202163,
        964090, 1166253, 4462849, 14554800
    };

    /* if the cache is not large enough, grow it */
    if (cap <= (size_t)n) {
        /* grow to at least n+1, but ensure we have size 16 for init */
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 16) new_cap = 16;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;

        /* fill the first 16 entries if they are missing */
        for (size_t i = 0; i < 16 && i < new_cap; ++i)
            cache[i] = init[i];

        /* compute any new terms using the recurrence */
        size_t start = (cap > 16) ? cap : 16;
        for (size_t i = start; i <= (size_t)n; ++i)
            cache[i] = coeff * cache[i - 8] - cache[i - 16];

        cap = new_cap;
    }

    return cache[(size_t)n];
}
