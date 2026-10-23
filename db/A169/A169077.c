/*
 * A169077  Number of reduced words of length n in Coxeter group on 16
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^25 = I.
 * https://oeis.org/A169077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A169077
   a(0) = 1
   For n >= 1:
     let coeff(n) = 2   for 1 <= n <= 24
                  = 1   for n = 25
                  = 0   for n >= 26
   a(n) = 14 * (a(n-1)+a(n-2)+...+a(n-24))
          - 105 * a(n-25)   (with a(k)=0 if k<0)
          + coeff(n)
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of all computed values up to the largest n seen */
    static seq_t *tab = NULL;   /* array of values */
    static idx_t  computed = 0; /* largest index already computed */

    /* Ensure tab has space for indices 0..n */
    if (n >= computed) {
        idx_t new_size = n + 1;                      /* need indices 0..n */
        if (tab == NULL) {
            tab = (seq_t *)malloc(sizeof(seq_t) * new_size);
        } else {
            tab = (seq_t *)realloc(tab, sizeof(seq_t) * new_size);
        }
        if (tab == NULL) {
            /* allocation failure – abort or handle appropriately */
            exit(1);
        }

        /* initialise the first element if this is the first call */
        if (computed == 0) {
            tab[0] = 1;
        }

        /* compute values from the next index up to n */
        for (idx_t k = computed + 1; k <= n; ++k) {
            seq_t sum24 = 0;
            for (idx_t j = 1; j <= 24 && k >= j; ++j) {
                sum24 += tab[k - j];
            }

            seq_t val = sum24 * 14;            /* 14 * sum of last 24 terms */

            if (k >= 25) {
                val -= tab[k - 25] * 105;       /* -105 * a(k-25) */
            }

            /* add the coefficient term */
            if (k <= 24) {
                val += 2;                      /* coeff = 2 for 1..24 */
            } else if (k == 25) {
                val += 1;                      /* coeff = 1 for n = 25 */
            }
            /* coeff = 0 for n >= 26 – nothing to add */

            tab[k] = val;
        }
        computed = n;
    }

    return tab[n];
}
