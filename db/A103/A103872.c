/*
 * A103872  a(n) = 3*trinomial(n+1,0) - trinomial(n+2,0).
 * https://oeis.org/A103872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*trinomial(n+1,0) – trinomial(n+2,0) * Central trinomial
 *            coefficient T(m)=binom(m,0)*binom(m,0)+...+binom(m,
 *            floor(m/2))*binom(m – floor(m/2), floor(m/2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   seq_malloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_malloc.h"

static seq_t *cent = NULL;          /* cache of central trinomial coefficients */
static idx_t cent_size = 0;         /* number of cached values */

/* Compute T(m) – central trinomial coefficient for index m. */
static seq_t compute_center(idx_t m)
{
    if (m >= cent_size) {
        /* grow cache up to m */
        size_t newsize = (size_t)m + 1;
        seq_t *newptr = seq_malloc(newsize);
        if (cent) {
            for (idx_t i = 0; i < cent_size; ++i) newptr[i] = cent[i];
        }

        /* compute new values */
        for (idx_t i = cent_size; i <= m; ++i) {
            seq_t sum = 0;
            seq_t term = 1;
            idx_t maxk = i / 2;
            for (idx_t k = 0; k <= maxk; ++k) {
                sum += term;
                if (k < maxk) {
                    seq_t num1 = (seq_t)(i - 2 * k);
                    seq_t num2 = (seq_t)(i - 2 * k - 1);
                    seq_t den  = (seq_t)(k + 1);
                    den = den * den;
                    term = term * num1 * num2 / den;   /* exact division */
                }
            }
            newptr[i] = sum;
        }

        cent_size = newsize;
        if (cent) free(cent);
        cent = newptr;
    }
    return cent[m];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t t1 = compute_center(n + 1);
    seq_t t2 = compute_center(n + 2);
    return (seq_t)3 * t1 - t2;
}
