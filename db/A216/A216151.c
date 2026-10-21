/*
 * A216151  a(n+1) = (Product_{k=1..n} a(k)) * Sum_{k=1..n} a(k), a(1)=1,
 *          a(2)=2.
 * https://oeis.org/A216151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1, a(2) = 2; for n ≥ 3,
   a(n) = (∏_{k=1}^{n-1} a(k)) * (∑_{k=1}^{n-1} a(k)) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *A = NULL;          /* stored values a(k)  */
    static seq_t *S = NULL;          /* partial sums Σ_{k=1}^i a(k)  */
    static seq_t *P = NULL;          /* partial products ∏_{k=1}^i a(k)  */
    static idx_t cap = 0;            /* allocated capacity            */
    static idx_t len = 0;            /* highest n that has been computed */

    if (n == 0) return (seq_t)0;     /* not part of the sequence */

    /* grow the arrays if needed */
    if (n >= cap) {
        idx_t new_cap = n + 1;                           /* +1 to use 1‑based indices */
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        S = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
        P = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute all missing values up to n */
    for (idx_t k = len + 1; k <= n; ++k) {
        if (k == 1) {
            A[(size_t)1] = (seq_t)1;
            S[(size_t)1] = (seq_t)1;
            P[(size_t)1] = (seq_t)1;
        } else if (k == 2) {
            A[(size_t)2] = (seq_t)2;
            S[(size_t)2] = S[(size_t)1] + A[(size_t)2];
            P[(size_t)2] = P[(size_t)1] * A[(size_t)2];
        } else {
            A[(size_t)k] = P[(size_t)(k-1)] * S[(size_t)(k-1)];
            S[(size_t)k] = S[(size_t)(k-1)] + A[(size_t)k];
            P[(size_t)k] = P[(size_t)(k-1)] * A[(size_t)k];
        }
    }

    len = n;
    return A[(size_t)n];
}
