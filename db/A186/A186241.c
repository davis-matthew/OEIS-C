/*
 * A186241  G.f. satisfies A(x) = 1 + x*A(x)^2 + x^2*A(x)^4 + x^3*A(x)^6.
 * https://oeis.org/A186241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A186241
   Recurrence derived from the functional equation
   A(x) = 1 + x*A(x)^2 + x^2*A(x)^4 + x^3*A(x)^6
   For n ≥ 1,
     a(n) = c2[n‑1] + c4[n‑2] + c6[n‑3]
   where
     c2[k] = coeff of x^k in A(x)^2,
     c4[k] = coeff of x^k in A(x)^4 = Σ_{i=0}^{k} c2[i]*c2[k-i],
     c6[k] = coeff of x^k in A(x)^6 = Σ_{i=0}^{k} c3[i]*c3[k-i],
     c3[k] = coeff of x^k in A(x)^3 = Σ_{i=0}^{k} a[i]*c2[k-i].
   The arrays a, c2 and c3 are built incrementally; the algorithm runs
   in O(n^2) time and O(n) memory. */

#include <stddef.h>
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *A = NULL;      /* a[0..cap-1]            */
    static seq_t *C2 = NULL;     /* coeffs of A^2          */
    static seq_t *C3 = NULL;     /* coeffs of A^3          */
    static size_t cap = 0;       /* allocation capacity    */
    static size_t last = 0;      /* highest index already computed */

    /* initialise arrays lazily */
    if (cap == 0) {
        cap = 16;
        A  = seq_realloc(NULL, 0, cap);
        C2 = seq_realloc(NULL, 0, cap);
        C3 = seq_realloc(NULL, 0, cap);
        for (size_t i = 0; i < cap; ++i) {
            A[i]  = 0;
            C2[i] = 0;
            C3[i] = 0;
        }
        A[0]  = 1;               /* a0 = 1                 */
        C2[0] = 1;               /* coeff of x^0 in A^2    */
        C3[0] = 1;               /* coeff of x^0 in A^3    */
        last = 0;
    }

    /* grow arrays if needed */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        A  = seq_realloc(A,  cap, new_cap);
        C2 = seq_realloc(C2, cap, new_cap);
        C3 = seq_realloc(C3, cap, new_cap);
        for (size_t i = cap; i < new_cap; ++i) {
            A[i]  = 0;
            C2[i] = 0;
            C3[i] = 0;
        }
        cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (size_t k = last + 1; k <= (size_t)n; ++k) {
        /* coefficient of x^{k-1} in A^2 */
        seq_t s2 = (k >= 1) ? C2[k - 1] : 0;

        /* coefficient of x^{k-2} in A^4 */
        seq_t s4 = 0;
        if (k >= 2) {
            for (size_t i = 0; i <= k - 2; ++i)
                s4 += C2[i] * C2[k - 2 - i];
        }

        /* coefficient of x^{k-3} in A^6 */
        seq_t s6 = 0;
        if (k >= 3) {
            for (size_t i = 0; i <= k - 3; ++i)
                s6 += C3[i] * C3[k - 3 - i];
        }

        /* a[k] from the functional equation */
        A[k] = s2 + s4 + s6;

        /* update C2[k] = coeff of x^k in A^2 */
        seq_t tmp2 = 0;
        for (size_t i = 0; i <= k; ++i)
            tmp2 += A[i] * A[k - i];
        C2[k] = tmp2;

        /* update C3[k] = coeff of x^k in A^3 */
        seq_t tmp3 = 0;
        for (size_t i = 0; i <= k; ++i)
            tmp3 += A[i] * C2[k - i];
        C3[k] = tmp3;
    }

    last = (size_t)n;
    return A[n];
}
