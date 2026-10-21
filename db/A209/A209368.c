/*
 * A209368  Number of n X 2 1..3 arrays with no element with value z exactly a
 *          city block distance of z from another element with value z.
 * https://oeis.org/A209368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+3*a(n-3)+a(n-4)-a(n-5)-a(n-6) (n>11)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 56, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=8,a(2)=39,a(3)=84,a(4)=129,a(5)=178,a(6)=338,a(7)=722,*/
/*                 a(8)=1568,a(9)=3362,a(10)=7200,a(11)=15488              */

#include <stdlib.h>

/* First 11 terms of the sequence */
static const seq_t init_values[11] = {
    /* a(1) to a(11) */
    ((seq_t)8),    ((seq_t)39),   ((seq_t)84),   ((seq_t)129),
    ((seq_t)178),  ((seq_t)338),  ((seq_t)722),  ((seq_t)1568),
    ((seq_t)3362), ((seq_t)7200), ((seq_t)15488)
};

seq_t a(idx_t n)
{
    /* Cache for already computed terms */
    static seq_t *cache = NULL;   /* cache[0] unused; cache[i] holds a(i) */
    static idx_t max_comp = 0;    /* highest index computed and stored */

    if (n == 0)  /* offset is 1, return 0 for invalid index */
        return (seq_t)0;

    /* Initialise cache if first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc((11 + 1) * sizeof(seq_t));  /* indices 0..11 */
        if (!cache) return (seq_t)0;  /* allocation failed – unlikely */
        for (idx_t i = 1; i <= 11; ++i)
            cache[i] = init_values[i - 1];
        max_comp = 11;
    }

    /* If requested index is already computed */
    if (n <= max_comp)
        return cache[n];

    /* Extend the cache array to accommodate up to n */
    if (cache != NULL) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed – unlikely */
        cache = tmp;
    } else {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    /* Compute terms from max_comp+1 up to n (inclusive) */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i <= 11) {
            /* These values are already stored in init_values,
               but we recompute in case the cache was reallocated. */
            cache[i] = init_values[i - 1];
        } else {
            cache[i] =
                cache[i - 1] + cache[i - 2] + 3 * cache[i - 3] + cache[i - 4]
                - cache[i - 5] - cache[i - 6];
        }
        max_comp = i;
    }

    return cache[n];
}
