/*
 * A190591  The coefficient of t^n in the power series solution of u in the
 *          equation -t+(1-t+t^2+t^3)*u-(t+t^4)*u^2+(t^3+t^5)*u^3-t^4*u^4=0.
 * https://oeis.org/A190591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 33, u64 n <= 61, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the equation
 *  -t + (1-t+t²+t³)u – (t+t⁴)u² + (t³+t⁵)u³ – t⁴u⁴ = 0.
 *
 *  a(0) = 0, a(1) = 1.
 *  For n ≥ 2:
 *    a(n) = a(n-1) - a(n-2) - a(n-3)
 *           + b(n-1) + b(n-4)
 *           - c(n-3) - c(n-5)
 *           + d(n-4)
 *  where
 *    b(k) = Σ_{i=0}^k a(i)·a(k-i)
 *    c(k) = Σ_{i=0}^k a(i)·b(k-i)
 *    d(k) = Σ_{i=0}^k a(i)·c(k-i)
 */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Memoisation storage */
    static idx_t cap      = 0;   /* allocated array size (≥ n+1) */
    static idx_t max_comp = 0;   /* highest n already computed (≥0)   */
    static seq_t *A = NULL;
    static seq_t *B = NULL;      /* b array */
    static seq_t *C = NULL;      /* c array */
    static seq_t *D = NULL;      /* d array */

    /* Ensure storage for indices 0 … n */
    if ((idx_t)(n + 1) > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmpA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        seq_t *tmpB = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
        seq_t *tmpC = (seq_t *)realloc(C, new_cap * sizeof(seq_t));
        seq_t *tmpD = (seq_t *)realloc(D, new_cap * sizeof(seq_t));
        if (!tmpA || !tmpB || !tmpC || !tmpD) abort();
        if (cap < new_cap) {
            memset(tmpA + cap, 0, (new_cap - cap) * sizeof(seq_t));
            memset(tmpB + cap, 0, (new_cap - cap) * sizeof(seq_t));
            memset(tmpC + cap, 0, (new_cap - cap) * sizeof(seq_t));
            memset(tmpD + cap, 0, (new_cap - cap) * sizeof(seq_t));
        }
        A = tmpA; B = tmpB; C = tmpC; D = tmpD;
        cap = new_cap;
    }

    /* Initialise base value a(0) = 0 (already zeroed on first call) */
    if (max_comp == 0 && cap >= 1) {
        A[0] = 0;
        B[0] = 0;
        C[0] = 0;
        D[0] = 0;
        max_comp = 0;
    }

    /* Compute all terms up to n */
    for (idx_t cur = max_comp + 1; cur <= n; ++cur) {
        if (cur == 1) {
            A[cur] = 1;                     /* a(1) = 1 */
        } else {
            seq_t val = A[cur - 1];
            if (cur >= 2) val -= A[cur - 2];
            if (cur >= 3) val -= A[cur - 3];
            if (cur >= 1) val += B[cur - 1];
            if (cur >= 4) val += B[cur - 4];
            if (cur >= 3) val -= C[cur - 3];
            if (cur >= 5) val -= C[cur - 5];
            if (cur >= 4) val += D[cur - 4];
            A[cur] = val;
        }

        /* Update convolution arrays for the new index */
        /* b[cur] = Σ a[i] * a[cur-i] */
        seq_t sum_b = 0;
        for (idx_t i = 0; i <= cur; ++i)
            sum_b += A[i] * A[cur - i];
        B[cur] = sum_b;

        /* c[cur] = Σ a[i] * b[cur-i] */
        seq_t sum_c = 0;
        for (idx_t i = 0; i <= cur; ++i)
            sum_c += A[i] * B[cur - i];
        C[cur] = sum_c;

        /* d[cur] = Σ a[i] * c[cur-i] */
        seq_t sum_d = 0;
        for (idx_t i = 0; i <= cur; ++i)
            sum_d += A[i] * C[cur - i];
        D[cur] = sum_d;
    }

    max_comp = n;
    return A[n];
}
