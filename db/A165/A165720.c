/*
 * A165720  Integers of the form k*(k+11)/10.
 * https://oeis.org/A165720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) - a(n-4) + a(n-5); Initial
 *            values: a(1)=6, a(2)=8, a(3)=18, a(4)=21, a(5)=35. The
 *            recurrence is valid for all n >= 6
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* Base values for n = 1..5 */
    static const seq_t base[6] = { 0, 6, 8, 18, 21, 35 };

    /* Cached sequence values */
    static seq_t *cache = NULL;
    static idx_t last_computed = 0;

    /* Allocate or grow cache to at least n+1 elements */
    if (cache == NULL) {
        size_t cap = (size_t)(n + 1 > 6 ? n + 1 : 6);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        /* initialise first elements (index 0 unused) */
        for (idx_t i = 1; i <= 5; ++i) cache[i] = base[i];
        last_computed = 5;
    }
    else if (last_computed < n) {
        /* expand cache if necessary */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        /* compute new terms */
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            /* a(i) = a1 + 2*a2 - 2*a3 - a4 + a5 */
            seq_t val = a1;
            val += a2;
            val += a2;   /* 2*a2 */
            val += a5;   /* + a5 */
            val -= a3;
            val -= a3;   /* -2*a3 */
            val -= a4;   /* -a4 */
            cache[i] = val;
        }
        last_computed = n;
    }

    return cache[n];
}
