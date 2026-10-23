/*
 * A153155  Coefficients of the eighth-order mock theta function T_0(q).
 * https://oeis.org/A153155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in Σ_{m≥0} q^{(m+1)(m+2)} ∏_{k=1}^m
 *            (1+q^{2k}) / ∏_{k=0}^m (1+q^{2k+1})
 * Range:     i8 n <= 32, i16 n <= 91, i32 n <= 288, i64 n <= 991,
 *            i128 n <= 3617, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation evaluates the sum up to the required n, while
   maintaining the series for P_m/D_m incrementally. */
#include <stdlib.h>
#include <stdint.h>

/* External types: seq_t, idx_t are supplied by the build system */
seq_t a(idx_t n)
{
    /* Allocate arrays of length n+1 to hold coefficients up to n. */
    idx_t  N   = n;
    size_t sz  = (size_t)(N + 1);
    seq_t *A    = (seq_t*)calloc(sz, sizeof(seq_t));   /* final accumulated series */
    seq_t *R    = (seq_t*)calloc(sz, sizeof(seq_t));   /* current P_m/D_m series */
    seq_t *Rnext= (seq_t*)calloc(sz, sizeof(seq_t));   /* temporary for next m */

    if (!A || !R || !Rnext) {
        free(A); free(R); free(Rnext);
        return (seq_t)0;
    }

    /* ---- m = 0 :  P_0 = 1, D_0 = 1 + q  */
    /* 1/(1+q) expands as alternating 1,-1,1,-1,…  */
    for (idx_t i = 0; i <= N; ++i) {
        R[i] = (i & 1) ? (seq_t)(-1) : (seq_t)1;
    }

    /* shift for m = 0 : s0 = (0+1)(0+2) = 2 */
    if (N >= 2) {
        for (idx_t i = 0; i + 2 <= N; ++i) {
            A[i + 2] += R[i];
        }
    }

    /* ---- iterate for m = 1, 2, … ---------------------------------- */
    for (idx_t m = 0; ; ++m) {
        idx_t next_m = m + 1;
        idx_t s_next = (next_m + 1) * (next_m + 2);   /* shift for this m */

        if (s_next > N)                  /* no more contributions needed */
            break;

        idx_t e = 2 * next_m;             /* factor (1 + q^{2*next_m})  */
        idx_t d = e + 1;                  /* factor (1 + q^{2*next_m+1}) */

        /* Rnext = R * (1 + q^e) */
        for (idx_t i = 0; i <= N; ++i) {
            seq_t val = R[i];
            if (i >= e)
                val += R[i - e];
            Rnext[i] = val;
        }

        /* divide by (1 + q^d):
           for all i >= d:  Rnext[i] -= Rnext[i-d]   (inverse convolution) */
        for (idx_t i = d; i <= N; ++i) {
            Rnext[i] -= Rnext[i - d];
        }

        /* shift the result by s_next and add to the final series A */
        for (idx_t i = 0; i + s_next <= N; ++i) {
            A[s_next + i] += Rnext[i];
        }

        /* ready for next iteration */
        seq_t *tmp = R;
        R = Rnext;
        Rnext = tmp;
    }

    seq_t result = A[N];

    free(A);
    free(R);
    free(Rnext);
    return result;
}
