/*
 * A191412  G.f. satisfies: A(x) = x*exp( Sum_{n>=1} A(x^n/(1-x^n))/n ).
 * https://oeis.org/A191412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived recurrence for OEIS A191412
 * a(1) = 1
 * For n >= 2:
 *   e[0] = 1
 *   e[n-1] = (1/(n-1)) * Σ_{k=1}^{n-1} c[k] * e[n-1-k]
 *   a[n]       = e[n-1]
 *   inner[n]   = Σ_{m=1}^{n} a[m] * binom(n-1, m-1)
 *   c[n]       = Σ_{t|n} t * inner[t]
 *
 *  a[n] = e[n-1] is the desired term.
 *  All arithmetic uses only integer operations on the type seq_t.
 */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache of computed values */
    static idx_t cached = 0;
    static seq_t *A = NULL;     /* a[1..]          */
    static seq_t *E = NULL;     /* e[0..]          */
    static seq_t *inner = NULL; /* inner[1..]      */
    static seq_t *c = NULL;     /* c[1..]          */

    /* Allocate / expand cache if necessary */
    if (cached < n) {
        idx_t new_sz = n;
        A = (seq_t*)realloc(A, sizeof(seq_t)*(new_sz+1));
        E = (seq_t*)realloc(E, sizeof(seq_t)*(new_sz+1));
        inner = (seq_t*)realloc(inner, sizeof(seq_t)*(new_sz+1));
        c = (seq_t*)realloc(c, sizeof(seq_t)*(new_sz+1));
        if (cached == 0) {
            /* Initialise for n = 1 */
            A[1] = (seq_t)1;
            inner[1] = (seq_t)1;             /* Σ_{m=1}^1 a[m]*binom(0,m-1) = a1*1 = 1 */
            c[1] = (seq_t)1 * inner[1];      /* 1 * inner[1] = 1 */
            E[0] = (seq_t)1;                 /* e[0] = 1 */
            cached = 1;
        }
        /* Compute terms from cached+1 up to n */
        for (idx_t i = cached + 1; i <= n; ++i) {
            /* 1. Compute e[i-1] = (1/(i-1)) * Σ_{k=1}^{i-1} c[k]*E[i-1-k] */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                sum += c[k] * E[i-1 - k];
            }
            E[i-1] = sum / (i-1);
            A[i] = E[i-1];

            /* 2. Compute inner[i] = Σ_{m=1}^{i} a[m] * binom(i-1, m-1) */
            seq_t inner_val = (seq_t)0;
            seq_t bin = (seq_t)1;   /* binom(i-1,0) = 1 */
            for (idx_t m = 1; m <= i; ++m) {
                inner_val += A[m] * bin;
                if (m < i) {
                    /* binom(i-1, m) = binom(i-1, m-1) * (i-1-(m-1))/m
                     *   = bin * (i-m)/m   */
                    bin = bin * (i - m) / m;
                }
            }
            inner[i] = inner_val;

            /* 3. Compute c[i] = Σ_{t|i} t * inner[t] */
            seq_t ci = (seq_t)0;
            for (idx_t d = 1; d*d <= i; ++d) {
                if (i % d == 0) {
                    idx_t t1 = d;
                    ci += t1 * inner[t1];
                    idx_t t2 = i / d;
                    if (t2 != t1) {
                        ci += t2 * inner[t2];
                    }
                }
            }
            c[i] = ci;
        }
        cached = n;
    }
    return A[n];
}
