/*
 * A170384  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^43 = I.
 * https://oeis.org/A170384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170384
   Computed via the linear recurrence derived from
   G(t) = (t^43 + 2 t^42 + … + 2 t + 1) /(1 - 37 t - 37 t^2 - … - 37 t^42 + 703 t^43)
   with a(0) = 1.  The recurrence for the auxiliary series b is
        b[0] = 1
        b[n] = 37·b[n-1] + … + 37·b[n-42] – 703·b[n-43]   (n ≥ 1)
   and the sequence a is obtained by convolution with the polynomial
        P(t) = 1 + 2 t + … + 2 t^42 + t^43   :
        a[n] = b[n] + 2·b[n-1] + … + 2·b[n-42] + b[n-43]   (n ≥ 0).
   The function is memoised and works for any sequence type seq_t
   (ordinary integer or arbitrary-precision) and index type idx_t. */

#include <stdlib.h>

/* Replace seq_t and idx_t with the types defined in the build. */
seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *b = NULL;      /* coefficients of the auxiliary series */
    static seq_t *A = NULL;      /* a(n) values */
    static idx_t cap = 0;        /* allocated capacity */
    static idx_t last = 0;       /* largest n computed so far */
    static int initialized = 0;  /* flag marking first initialisation */

    /* ensure at least one element is allocated and initialise a(0) once */
    if (!initialized) {
        /* allocate for only a(0) initially */
        cap = 1;
        b = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        A = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!b || !A) abort();          /* allocation failure */
        b[0] = (seq_t)1;
        A[0] = (seq_t)1;
        last = 0;
        initialized = 1;
    }

    /* grow storage if necessary */
    if ((idx_t)(cap) <= n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_b = (seq_t *)realloc(b, new_cap * sizeof(seq_t));
        seq_t *new_A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!new_b || !new_A) abort();
        b = new_b;
        A = new_A;
        cap = new_cap;
    }

    /* compute terms from last+1 up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* compute b[i] */
        seq_t sum = 0;
        /* k = 1 .. 42 */
        for (idx_t k = 1; k <= 42 && k <= i; ++k) {
            sum += (seq_t)37 * b[i - k];
        }
        /* k = 43 */
        if (i >= 43) {
            sum -= (seq_t)703 * b[i - 43];
        }
        b[i] = sum;

        /* compute A[i] = convolution of P with b */
        seq_t value = b[i];              /* p0 * b[i]  (p0 = 1) */
        if (i >= 1) {
            value += (seq_t)2 * b[i - 1]; /* p1 = 2 */
        }
        for (idx_t k = 2; k <= 42 && k <= i; ++k) {
            value += (seq_t)2 * b[i - k]; /* p2..p42 = 2 */
        }
        if (i >= 43) {
            value += b[i - 43];           /* p43 = 1 */
        }
        A[i] = value;
    }
    last = n;
    return A[n];
}
