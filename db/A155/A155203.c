/*
 * A155203  G.f.: A(x) = exp( Sum_{n>=1} 3^(n^2) * x^n/n ), a power series in
 *          x with integer coefficients.
 * https://oeis.org/A155203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = (1/n) * sum_{k=1..n} 3^(k^2) * a(n-k) for n >=
 *            1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This recurrence follows from the generating function
   A(x) = exp( Σ_{k≥1} 3^(k^2) x^k / k ).           */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"

static seq_t *A = NULL;          /* a(n) values */
static seq_t *P = NULL;          /* 3^{k^2} values */
static size_t cap = 0;           /* currently allocated capacity – covers indices 0..cap   */
static idx_t max_computed = 0;   /* highest n already computed */

/* Ensure arrays large enough to index up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cap == 0) {
        /* allocate space for index 0 at least */
        cap = 1;
        A = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        P = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        for (size_t i = 0; i <= cap; ++i) {
            A[i] = 0;
            P[i] = 0;
        }
    }
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* grow until > n */

        A = (seq_t *)realloc(A, (new_cap + 1) * sizeof(seq_t));
        P = (seq_t *)realloc(P, (new_cap + 1) * sizeof(seq_t));

        for (size_t i = cap + 1; i <= new_cap; ++i) {
            A[i] = 0;
            P[i] = 0;
        }
        cap = new_cap;
    }
}

/* public function */
seq_t a(idx_t n)
{
    ensure_capacity(n);

    /* set a(0)=1 if not already */
    if (n == 0) {
        if (A[0] == 0) A[0] = 1;
        if (max_computed < n) max_computed = n;
        return A[0];
    }

    /* compute missing terms up to n */
    if (n > max_computed) {
        if (max_computed == 0) {
            /* a(0) already set in ensure_capacity */
            A[0] = 1;
            max_computed = 0;
        }

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* compute 3^{i^2}  (big_pow is available via utils/big_pow.h) */
            seq_t exp = (seq_t)(i * i);          /* i*i fits in idx_t */
            P[i] = big_pow((seq_t)3, exp);

            /* compute the convolution sum */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) {
                sum += P[k] * A[i - k];
            }
            A[i] = sum / (seq_t)i;               /* exact division */
        }
        max_computed = n;
    }

    return A[n];
}
