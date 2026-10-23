/*
 * A156213  G.f.: A(x) = exp( Sum_{n>=1} 2^(n^2)*C(2*n-1,n)*x^n/n ), a power
 *          series in x with integer coefficients.
 * https://oeis.org/A156213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n) = (1/n) * Σ_{k=1..n} 2^{k^2} * C(2k-1,k) * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   binomial, pow2, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence derived from the generating function
 *  A(x) = exp( Σ_{n≥1} 2^{n^2} * C(2n-1,n) * x^n / n ).  */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *A = NULL;       /* array of already computed a(i) */
    static size_t A_cap = 0;      /* current capacity of A (number of elements) */

    /* if we need more space, grow the cache */
    if ((size_t)n + 1 > A_cap) {
        size_t old_cap = A_cap;
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* memory exhausted */
        A = tmp;
        A_cap = new_cap;

        /* initialize the first term if this is the very first allocation */
        if (old_cap == 0) {
            A[0] = seq_one();       /* a(0) = 1 */
        }
    }

    /* compute all new values up to n */
    for (idx_t i = (idx_t)(A_cap - 1); i <= n; ++i) {
        /* already computed a(0) */
        if (i == 0) continue;

        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            /* coefficient term = 2^{k^2} * C(2k-1,k) */
            seq_t coef = pow2(k * k) * binomial(2 * k - 1, k);
            sum += coef * A[i - k];
        }
        A[i] = sum / (seq_t)i;     /* divide by i (exact division) */
    }

    return A[n];
}
