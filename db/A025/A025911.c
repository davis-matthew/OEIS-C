/*
 * A025911  Expansion of 1/((1-x^7)*(1-x^8)*(1-x^12)).
 * https://oeis.org/A025911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-7)+a(n-8)+a(n-12)-a(n-15)-a(n-19)-a(n-20)+a(n-27)
 *            Initial terms: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0,
 *            a(6)=0 (All other values are computed by the recurrence)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache to store computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;      /* allocated size of cache */
    static idx_t  last  = 0;      /* highest index whose value is known */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* allocate enough space for the initial known terms (up to 27) */
        size_t init = 28;
        if (n + 1 > init) init = n + 1;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);

        /* zero all entries; the recurrence needs them */
        memset(cache, 0, init * sizeof(seq_t));

        /* known initial value */
        cache[0] = 1;

        cap  = init;
        last = 0;
    }

    /* extend cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        /* zero the newly added entries */
        memset(tmp + cap, 0, (newcap - cap) * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing values up to n */
    if ((idx_t)last < n) {
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            seq_t val = 0;

            if (i >= 7)  val += cache[i - 7];
            if (i >= 8)  val += cache[i - 8];
            if (i >= 12) val += cache[i - 12];
            if (i >= 27) val += cache[i - 27];

            seq_t sub = 0;
            if (i >= 15) sub += cache[i - 15];
            if (i >= 19) sub += cache[i - 19];
            if (i >= 20) sub += cache[i - 20];

            val -= sub;              /* the result is guaranteed non‑negative */
            cache[i] = val;
        }
        last = n;
    }

    return cache[(size_t)n];
}
