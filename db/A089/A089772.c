/*
 * A089772  a(n) = Lucas(11*n).
 * https://oeis.org/A089772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 199*a(n-1) + a(n-2) a(0) = 2, a(1) = 199 (Lucas(0) = 2,
 *            Lucas(11) = 199)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc */

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Static cache: cache[0..computed-1] store values a(0..computed-1). */
    static seq_t *cache = NULL;
    static idx_t computed = 0;   /* number of values already computed (max index+1) */
    static idx_t cap      = 0;   /* allocated capacity */

    /* Ensure capacity for indices up to n. */
    if (n + 1 > cap)
    {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)
            return (seq_t)0;           /* allocation failure → return 0 */
        cache = tmp;
        cap = new_cap;
    }

    /* If nothing has been computed yet, set the first two terms. */
    if (computed == 0)
    {
        if (cap > 0) { cache[0] = (seq_t)2; }
        if (cap > 1) { cache[1] = (seq_t)199; }
        computed = (cap >= 2) ? 2 : cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)2;
        }
        else if (i == 1)
        {
            cache[1] = (seq_t)199;
        }
        else
        {
            /* a(i) = 199*a(i-1) + a(i-2) */
            cache[i] = cache[i - 1] * (seq_t)199 + cache[i - 2];
        }
    }

    /* Update the number of computed values. */
    computed = n + 1;

    return cache[n];
}
