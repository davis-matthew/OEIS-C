/*
 * A066986  Integers of the form (Product_{i=1..k} sigma(i))/(Product_{i=1..k}
 *          phi(i)) = A066780(k)/A001088(k).
 * https://oeis.org/A066986
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   free_seq_array, phi, seq_alloc_new_array, seq_gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A066986: a(n) = ∏_{i=1..k} σ(i) / ∏_{i=1..k} φ(i),
   where k is the n‑th positive integer for which the above quotient
   is an integer. */

#include <stddef.h>
#include "../utils/seq_gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/phi.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *values = NULL;         /* memoized sequence values              */
    static size_t values_cnt = 0;        /* number of values currently computed   */
    static size_t values_cap = 0;        /* allocated capacity of 'values'        */
    static idx_t  current_k  = 0;        /* last k processed                       */
    static seq_t  num = 1;               /* running product of σ(i)                */
    static seq_t  den = 1;               /* running product of φ(i)                */

    /* Generate terms until we have at least n of them */
    while (values_cnt < n) {
        /* Advance to the next k */
        current_k++;

        /* Compute σ(k) and φ(k) */
        seq_t sigma = sigma_of(current_k);
        seq_t phi_val = phi(current_k);

        /* Cancel common factors between σ(k) and the current denominator */
        seq_t g1 = seq_gcd(sigma, den);
        if (g1 > 1) {
            sigma /= g1;
            den   /= g1;
        }

        /* Cancel common factors between φ(k) and the current numerator */
        seq_t g2 = seq_gcd(phi_val, num);
        if (g2 > 1) {
            phi_val /= g2;
            num      /= g2;
        }

        /* Update the running products */
        num *= sigma;
        den *= phi_val;

        /* Reduce the fraction to lowest terms */
        seq_t g = seq_gcd(num, den);
        if (g > 1) {
            num /= g;
            den /= g;
        }

        /* If the denominator is 1, the quotient is an integer */
        if (den == 1) {
            /* Ensure enough capacity for the new value */
            if (values_cnt >= values_cap) {
                size_t new_cap = values_cap ? values_cap * 2 : 16;
                seq_t *new_vals = seq_alloc_new_array(new_cap);
                for (size_t i = 0; i < values_cnt; ++i)
                    new_vals[i] = values[i];
                if (values)
                    free_seq_array(values);
                values     = new_vals;
                values_cap = new_cap;
            }
            values[values_cnt++] = num;
        }
    }

    return values[n - 1];
}
