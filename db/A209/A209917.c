/*
 * A209917  E.g.f. A(x) satisfies: A(x/(1-x))/(1-x) = (1/x^2) * d/dx
 *          x^3*A(x)/3.
 * https://oeis.org/A209917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
  OEIS A209917
  Recurrence:
    a(0) = 1
    a(n) = (n-1)! * sum_{k=0}^{n-1} 3 * binomial(n,k) * a(k) / k!
  The following implementation computes a(n) using integer arithmetic only.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *A = NULL;          /* cache of sequence values */
static size_t ASIZE = 0;         /* current capacity of A */
static idx_t  maxComputed = 0;   /* largest index already computed */

/* ensure that A has space for at least indices 0..n */
static void ensure_capacity(idx_t n) {
    if (ASIZE <= (size_t)n) {
        size_t newSize = (n + 1) * 2;
        seq_t *tmp = (seq_t*)realloc(A, newSize * sizeof(seq_t));
        if (!tmp) abort();                     /* out of memory */
        /* initialise new slots to zero */
        for (size_t i = ASIZE; i < newSize; ++i) tmp[i] = (seq_t)0;
        A = tmp;
        ASIZE = newSize;
    }
}

seq_t a(idx_t n) {
    /* initialise cache on first call */
    if (ASIZE == 0) {
        ensure_capacity(0);
        A[0] = (seq_t)1;
        maxComputed = 0;
    }

    /* extend cache if needed */
    ensure_capacity(n);

    /* compute any missing terms up to n */
    for (idx_t i = maxComputed + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;          /* sum of terms for a[i] */
        seq_t binom = (seq_t)i;        /* C(i, i-1) */
        seq_t R = (seq_t)1;            /* (i-1)! / k! for current k */

        idx_t k = i - 1;
        while (1) {
            /* term = 3 * binom * R * a[k] */
            seq_t temp = A[k];
            temp = temp * R;            /* a[k] * R */
            temp = temp * binom;        /* * binom */
            temp = temp * (seq_t)3;     /* * 3 */
            sum = sum + temp;           /* accumulate */

            if (k == 0) break;         /* finished all k */

            /* update factors for next k-1 */
            R = R * (seq_t)k;                       /* R_{k-1} = R_k * k */
            seq_t numer = binom * (seq_t)k;         /* numerator = binom * k */
            seq_t denom = (seq_t)(i - k + 1);       /* denominator = i - k + 1 */
            binom = numer / denom;                  /* exact division */

            --k;
        }
        A[i] = sum;
    }
    maxComputed = n;
    return A[n];
}
