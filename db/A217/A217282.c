/*
 * A217282  G.f.: A(x) = exp( Sum_{n>=1} x^n/n * Sum_{k=0..n} binomial(n,k)^2
 *          * x^k*(1-x)^k ).
 * https://oeis.org/A217282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(n-2) + sum_{i=0}^{n-3} a(i) * a(n-3-i) for n>=3
 *            Initial values: a(0)=1, a(1)=1, a(2)=2 (derived from the
 *            algebraic generating function of A217282)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 32, u64 n <= 61, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for previously computed values */
    static seq_t *table = NULL;
    static idx_t cap = 0;   /* number of elements currently stored      */

    /* Ensure we have all values up to 'n' computed */
    if (n >= cap) {
        idx_t new_cap = n + 1;
        seq_t *new_table = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_table) abort();          /* allocation failure: terminate */

        /* Copy existing values */
        for (idx_t i = 0; i < cap; ++i)
            new_table[i] = table[i];

        /* Compute new values */
        for (idx_t k = cap; k < new_cap; ++k) {
            if (k < 3) {
                /* Set initial values for n = 0,1,2 */
                if (k == 0) new_table[0] = 1;
                else if (k == 1) new_table[1] = 1;
                else             new_table[2] = 2;
            } else {
                seq_t sum = 0;
                for (idx_t i = 0; i <= k - 3; ++i) {
                    sum += new_table[i] * new_table[k - 3 - i];
                }
                new_table[k] = sum + new_table[k - 2] + 1;
            }
        }

        /* Replace old table */
        free(table);
        table = new_table;
        cap   = new_cap;
    }

    return table[n];
}
