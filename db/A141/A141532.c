/*
 * A141532  Inverse binomial transform of A141425.
 * https://oeis.org/A141532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=1, a(2)=1, a(3)=-2, a(4)=4, a(5)=-8 a(n) = -
 *            (6*a(n-1)+15*a(n-2)+20*a(n-3)+15*a(n-4)+6*a(n-5)) for n≥6
 *            (derived from the rational generating function in the OEIS
 *            entry)
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: cache of computed values */
    static seq_t *cache      = NULL;   /* array holding a(0) .. a(cached_last) */
    static idx_t cache_cap   = 0;      /* allocated capacity (in elements)   */
    static idx_t cached_last = 5;      /* index of the largest computed val */

    /*---  Initialise base terms on first call  ---------------------------------*/
    if (cache_cap == 0)
    {
        cache_cap = 6;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)-2;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)-8;
        cached_last = 5;
    }

    /*---  Grow cache if necessary  ---------------------------------------------*/
    if (n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /*---  Return value if already computed  -------------------------------------*/
    if (n <= cached_last) return cache[n];

    /*---  Compute missing terms using the recurrence  ---------------------------*/
    for (idx_t i = cached_last + 1; i <= n; ++i)
    {
        cache[i] = - (
            ((seq_t)6 * cache[i-1]) +
            ((seq_t)15 * cache[i-2]) +
            ((seq_t)20 * cache[i-3]) +
            ((seq_t)15 * cache[i-4]) +
            ((seq_t)6 * cache[i-5])
        );
    }
    cached_last = n;

    return cache[n];
}
