/*
 * A198785  G.f. satisfies: A(x) = exp( Sum_{n>=1} A(x^n) / A(-x^n) * x^n/n ).
 * https://oeis.org/A198785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A198785:
   a(0) = 1
   The generating function satisfies
      A(x) = exp( Σ_{n≥1} A(xⁿ)/A(-xⁿ) * xⁿ/n )
   The algorithm builds A(x) coefficient by coefficient using
   a recursive convolution and divisor sums.  */
#include <stdlib.h>

/* The required interface */
seq_t a(idx_t n) {
    /* static data for memoisation */
    static seq_t *A = NULL;   /* a[0..]  */
    static seq_t *B = NULL;   /* b[0..]  */
    static seq_t *E = NULL;   /* e[0..]  */
    static size_t  size = 0;  /* allocated length of each array */
    static idx_t   next = 1;  /* next index that has not been computed yet */

    /* Lazily allocate space for at least (n+1) entries.
       We preallocate a bit more to reduce reallocs. */
    if (A == NULL) {
        size = (n + 1 > 4 ? n + 1 : 4);
        A = (seq_t *)malloc(size * sizeof(seq_t));
        B = (seq_t *)malloc(size * sizeof(seq_t));
        E = (seq_t *)malloc(size * sizeof(seq_t));

        /* initialise base case */
        A[0] = 1;
        B[0] = 1;
        E[0] = 0;
        next = 1;          /* next index to compute */
    } else if ((size_t)(n + 1) > size) {
        size_t new_size = (size_t)(n + 1) > size * 2 ? (size_t)(n + 1) : size * 2;
        A = (seq_t *)realloc(A, new_size * sizeof(seq_t));
        B = (seq_t *)realloc(B, new_size * sizeof(seq_t));
        E = (seq_t *)realloc(E, new_size * sizeof(seq_t));
        size = new_size;
    }

    /* Compute missing terms up to n */
    for (idx_t m = next; m <= n; ++m) {
        /* 1. compute b_{m-1} */
        idx_t j = m - 1;
        seq_t S = 0;
        for (idx_t k = 0; k < j; ++k) {
            seq_t term = B[k] * A[j - k];
            if (((j - k) & 1) != 0)   /* odd power of (-1) */
                S -= term;
            else
                S += term;
        }
        B[j] = A[j] - S;                 /* a_j is already known */

        /* 2. compute e_m = Σ_{d|m} b_{m/d-1} * (m/d) */
        seq_t e_val = 0;
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                idx_t d1 = d;
                idx_t d2 = m / d;
                seq_t part1 = B[m / d1 - 1] * (m / d1);
                e_val += part1;
                if (d2 != d1) {
                    seq_t part2 = B[m / d2 - 1] * (m / d2);
                    e_val += part2;
                }
            }
        }
        E[m] = e_val;

        /* 3. compute a_m from the exponential recurrence */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k)
            sum += E[k] * A[m - k];
        A[m] = sum / m;                  /* exact division */
    }
    next = n + 1;
    return A[n];
}
