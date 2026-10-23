/*
 * A191373  Sum of binomial coefficients C(i+j,i) modulo 2 over all pairs
 *          (i,j) of positive integers satisfying 5i+j=n.
 * https://oeis.org/A191373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 1 a(2*n) = a(n) a(2*n+1) = a(n) + a(n-2) (with
 *            a(k)=0 for k<0)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *table = NULL;
    static idx_t table_cap = 0;   /* capacity in elements */
    static idx_t max_computed = 1;/* highest index with a valid value */

    /* Ensure capacity for indices 0..max(n,1) */
    idx_t need = n;
    if (need < 1) need = 1;                 /* we need at least 0 and 1 */
    if (table_cap <= need) {
        idx_t new_cap = table_cap ? table_cap << 1 : 16;
        if (new_cap <= need) new_cap = need + 1;
        seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);            /* allocation failed */
        table = tmp;
        table_cap = new_cap;
        /* initialise base values if freshly allocated or missing */
        if (table_cap > 0) table[0] = 1;
        if (table_cap > 1) table[1] = 1;
    }

    /* If n is 0 or 1, it is already initialised */
    if (n <= 1) return table[n];

    /* Compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if ((i & (idx_t)1) == 0) {                /* even index */
            table[i] = table[i >> 1];
        } else {                                   /* odd index */
            idx_t k = (i - 1) >> 1;                 /* k = (i-1)/2 */
            seq_t val = table[k];
            if (k >= 2) {
                val += table[k - 2];
            }
            table[i] = val;
        }
    }

    max_computed = n > max_computed ? n : max_computed;
    return table[n];
}
