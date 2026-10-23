/*
 * A128098  Number of steps that touch the x-axis in all Motzkin paths of
 *          length n.
 * https://oeis.org/A128098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 12, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=4,
   a(n) = ( 2*n*(16*n^2-3*n-28)*a(n-1) + 3*n*(16*n-11)*(n-1)*a(n-2) )
          / ((n-1)*(16*n-27)*(n+4))  (valid for n ≥ 3) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)0;          /* unused, safety for 0-index */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    /* static cache */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;      /* highest index already filled */

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t init_cap = 3;                      /* indices 0,1,2 */
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;       /* allocation failed */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        max_computed = 2;
    }

    /* grow cache and compute missing values */
    if (n > max_computed) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;        /* allocation failed */
        cache = tmp;

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t si = (seq_t)i;
            seq_t s_i_1 = (seq_t)(i - 1);

            /* numerator part 1 */
            seq_t num1 = (seq_t)2 * si *
                         ((seq_t)16 * si * si - (seq_t)3 * si - (seq_t)28) *
                         cache[i - 1];

            /* numerator part 2 */
            seq_t num2 = (seq_t)3 * si *
                         ((seq_t)16 * si - (seq_t)11) *
                         s_i_1 * cache[i - 2];

            /* denominator */
            seq_t denom = s_i_1 *
                          ((seq_t)16 * si - (seq_t)27) *
                          (si + (seq_t)4);

            cache[i] = (num1 + num2) / denom;
        }
        max_computed = n;
    }

    return cache[n];
}
