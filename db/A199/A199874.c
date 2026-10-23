/*
 * A199874  G.f. satisfies A(x) = (1 + x*A(x)^2)*(1 + x^2*A(x)^2).
 * https://oeis.org/A199874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *   OEIS A199874
 *   Generating function:  A(x) = (1 + x*A(x)^2)*(1 + x^2*A(x)^2)
 *   Coefficients are obtained from the convolution recurrence:
 *        a_0 = 1,
 *        a_n = C[n-1] + C[n-2] + D[n-3]       for n ≥ 1
 *   where
 *        C[k] = Σ_{i+j=k} a_i * a_j   (coefficients of A(x)^2)
 *        D[k] = Σ_{p+q=k} C[p] * C[q] (coefficients of A(x)^4)
 *   All arrays are built incrementally.  The implementation keeps a
 *   static cache and expands it lazily, so calls may be in any order.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are provided by the build system.  */
seq_t a(idx_t n)
{
    /* Static cache for coefficients and intermediate convolutions. */
    static seq_t *a_vals = NULL;   /* a[0] … a[known-1]            */
    static seq_t *C_vals = NULL;   /* C[0] … C[known-1]            */
    static seq_t *D_vals = NULL;   /* D[0] … D[known-1]            */
    static idx_t  capacity = 0;    /* allocated size of each array  */
    static idx_t  known   = 0;     /* how many terms have been computed */

    /* Ensure the arrays are large enough to hold indices up to n. */
    if (n >= capacity) {
        idx_t new_size = (n + 1) * 2;   /* grow geometrically */
        a_vals = (seq_t *)realloc(a_vals, new_size * sizeof(seq_t));
        C_vals = (seq_t *)realloc(C_vals, new_size * sizeof(seq_t));
        D_vals = (seq_t *)realloc(D_vals, new_size * sizeof(seq_t));
        if (!a_vals || !C_vals || !D_vals) return 0;   /* allocation failed */

        /* initialise new slots to zero */
        for (idx_t i = capacity; i < new_size; ++i) {
            a_vals[i] = 0;
            C_vals[i] = 0;
            D_vals[i] = 0;
        }
        capacity = new_size;
    }

    /* Initialise the cache if this is the first call. */
    if (known == 0) {
        a_vals[0] = 1;              /* a(0) = 1 */
        C_vals[0] = a_vals[0] * a_vals[0];            /* C[0] = a0*a0 */
        D_vals[0] = C_vals[0] * C_vals[0];            /* D[0] = C0*C0 */
        known = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t m = known; m <= n; ++m) {
        seq_t term = 0;
        if (m >= 1) term += C_vals[m - 1];         /* coefficient of x^(m-1) in A^2 */
        if (m >= 2) term += C_vals[m - 2];         /* coefficient of x^(m-2) in A^2 */
        if (m >= 3) term += D_vals[m - 3];         /* coefficient of x^(m-3) in A^4 */
        a_vals[m] = term;

        /* Update C[m] = Σ_{i=0}^m a_i * a_{m-i} */
        seq_t sumC = 0;
        for (idx_t i = 0; i <= m; ++i) {
            sumC += a_vals[i] * a_vals[m - i];
        }
        C_vals[m] = sumC;

        /* Update D[m] = Σ_{p=0}^m C_p * C_{m-p} */
        seq_t sumD = 0;
        for (idx_t p = 0; p <= m; ++p) {
            sumD += C_vals[p] * C_vals[m - p];
        }
        D_vals[m] = sumD;
    }

    known = n + 1;  /* all terms up to n are now available */
    return a_vals[n];
}
