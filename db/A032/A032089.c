/*
 * A032089  "BHK" (reversible, identity, unlabeled) transform of
 *          1,0,1,0...(odds).
 * https://oeis.org/A032089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) - a(n-6) + a(n-7)
 *            + a(n-8) for n>8 Initial values: a(1)=1, a(2)=0, a(3)=1, a(4)=1,
 *            a(5)=2, a(6)=3, a(7)=5, a(8)=9
 * Range:     u8 n <= 14, u16 n <= 26, u32 n <= 49, u64 n <= 95, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array of computed values, 1‑based */
static idx_t last_computed = 0;  /* largest n for which cache[n] is valid */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined at offset, return 0 */

    /* ensure the cache holds space for indices up to n */
    size_t needed = (size_t)(n + 1);      /* index 0 is unused */
    if (!cache || needed > (size_t)(last_computed + 1)) {
        /* grow the array; realloc keeps existing data */
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        /* initialise new entries with zero so that the for‑loop below
           can distinguish uncomputed values from a legitimate 0 */
        for (size_t i = last_computed + 1; i <= n; ++i)
            cache[i] = 0;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* initialise the first eight values explicitly */
        if (i == 1)              cache[i] = (seq_t)1;
        else if (i == 2)         cache[i] = (seq_t)0;
        else if (i == 3)         cache[i] = (seq_t)1;
        else if (i == 4)         cache[i] = (seq_t)1;
        else if (i == 5)         cache[i] = (seq_t)2;
        else if (i == 6)         cache[i] = (seq_t)3;
        else if (i == 7)         cache[i] = (seq_t)5;
        else if (i == 8)         cache[i] = (seq_t)9;
        else { /* recurrence for n > 8 */
            seq_t val = cache[i - 1];                           /* a(n-1) */
            val += cache[i - 2] * ( (seq_t)3 );                /* +3*a(n-2) */
            val -= cache[i - 3] * ( (seq_t)2 );                /* -2*a(n-3) */
            val -= cache[i - 4] * ( (seq_t)2 );                /* -2*a(n-4) */
            val -= cache[i - 6];                              /* -a(n-6) */
            val += cache[i - 7];                              /* +a(n-7) */
            val += cache[i - 8];                              /* +a(n-8) */
            cache[i] = val;
        }
    }

    last_computed = n;
    return cache[n];
}
