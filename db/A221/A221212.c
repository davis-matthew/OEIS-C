/*
 * A221212  Rounded area of distinct right triangles appearing in the unit
 *          golden spiral.
 * https://oeis.org/A221212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) rounded x(n). Recurrence: a(1)=1, a(2)=3, a(3)=4, a(4)=7,
 *            a(5)=9 For n>5: if n even: a(n) = 3*a(n-2) - a(n-4) if n odd :
 *            a(n) = a(n-1) + a(n-3) The recurrence for even indices follows
 *            from the definition of the golden spiral, while odd terms are
 *            the sum of two preceding even terms before rou...
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 100,
 *            big any n
 * Helpers:   mult3_safe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mult3_safe.h"

seq_t a(idx_t n)
{
    if (n < 1)   /* Never called with n < 1, but guard anyway */
        return (seq_t)0;

    /* Base values a(1)..a(5) */
    const seq_t base[5] = { (seq_t)1,
                            (seq_t)3,
                            (seq_t)4,
                            (seq_t)7,
                            (seq_t)9 };

    /* Memoisation cache */
    static seq_t *cache = NULL;
    static idx_t   cache_len = 0;   /* number of terms stored */

    if (n <= cache_len)          /* already computed */
        return cache[n-1];

    /* Grow cache to hold terms up to n */
    idx_t new_cap = n;
    seq_t *new_cache;

    /* Use calloc for zero initialization; new copies will be overwritten */
    new_cache = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!new_cache)
        abort();

    /* Copy old terms */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Compute missing terms */
    for (idx_t idx = cache_len + 1; idx <= n; ++idx)
    {
        seq_t val;
        if (idx <= 5)
            val = base[idx - 1];
        else if ((idx & 1) == 0)               /* even index */
            val = mult3_safe(new_cache[idx - 3]) - new_cache[idx - 5];
        else                                   /* odd index, idx>=7 */
            val = new_cache[idx - 2] + new_cache[idx - 4];

        new_cache[idx - 1] = val;
    }

    /* Free old array */
    free(cache);

    cache = new_cache;
    cache_len = new_cap;

    return cache[n-1];
}
