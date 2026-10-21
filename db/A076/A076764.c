/*
 * A076764  n such that Omega(n-1) < Omega(n) < Omega(n+1), where Omega(m) =
 *          the number of prime factors of m, counting multiplicity.
 * https://oeis.org/A076764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 3954, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: OEIS A076764
   n such that Omega(n-1) < Omega(n) < Omega(n+1) */

#include <stdlib.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Static state: cache of terms, its capacity and length,
       plus the next integer to test. */
    static seq_t *terms   = NULL;
    static size_t terms_cap = 0, terms_len = 0;
    static idx_t current   = 2;         /* start checking from 2 */

    /* Ensure at least n terms have been found. */
    while (terms_len < (size_t)n) {
        seq_t om_prev = bigomega(current - 1);
        seq_t om_cur  = bigomega(current);
        seq_t om_next = bigomega(current + 1);

        if (om_prev < om_cur && om_cur < om_next) {
            /* Grow cache if necessary. */
            if (terms_len == terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) abort();   /* out of memory */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)current;
        }
        ++current;
    }

    return terms[n - 1];
}
