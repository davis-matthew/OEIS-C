/*
 * A028296  Expansion of e.g.f. Gudermannian(x) = 2*arctan(exp(x)) - Pi/2.
 * https://oeis.org/A028296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = -∑_{i=0}^{n-1} a(i)*C(2*n,2*i), a(0)=1
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 7, i64 n <= 11, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A028296 (Euler numbers E_{2n}) */
#include <stdlib.h>

static seq_t *A = NULL;          /* cache of computed terms */
static size_t A_cap = 0;         /* allocated capacity of A */
static idx_t cur_max = 0;        /* largest index computed, inclusive */

seq_t a(idx_t n)
{
    /* Allocate initial storage and set a(0) = 1 */
    if (A == NULL) {
        A_cap = 8;
        A = (seq_t*)malloc(A_cap * sizeof(seq_t));
        for (size_t i = 0; i < A_cap; ++i) A[i] = 0;
        A[0] = 1;
        cur_max = 0;
    }

    /* Grow storage if needed */
    if ((size_t)n >= A_cap) {
        size_t new_cap = A_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
        if (tmp == NULL) { /* allocation failed; fallback */
            return 0;
        }
        A = tmp;
        A_cap = new_cap;
    }

    /* Compute terms up to n on demand (in ascending order) */
    for (idx_t k = cur_max + 1; k <= n; ++k) {
        seq_t sum = 0;
        seq_t binom = 1;  /* C(2k,0) */
        for (idx_t i = 0; i < k; ++i) {
            sum += A[i] * binom;
            if (i < k - 1) {
                /* Compute next binomial coefficient: C(2k, 2i+2) */
                seq_t aik = ((seq_t)k)*2 - ((seq_t)i)*2;           /* 2k - 2i */
                seq_t num = aik * (aik - 1);                       /* (2k-2i)*(2k-2i-1) */
                seq_t den = ((seq_t)i*2 + 2) * ((seq_t)i*2 + 1);   /* (2i+2)*(2i+1) */
                binom = (binom * num) / den;
            }
        }
        A[k] = -sum;
    }
    cur_max = n;
    return A[n];
}
