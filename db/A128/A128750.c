/*
 * A128750  Number of skew Dyck paths of semilength n having no ascents of
 *          length 1.
 * https://oeis.org/A128750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = convolution recurrence * a(0) = 1, * a(1) = 0, * a(n) =
 *            Σ_{i=0}^{n-1} a[i] a[n-1-i] + Σ_{i=0}^{n-2} a[i] a[n-2-i] +
 *            a[n-2] (n ≥ 2) * * This recurrence follows from the quadratic
 *            equation * z(1+z)G^2 - (1 - z^2)G + 1 - z = 0 * satisfied by the
 *            generating function G(z)=∑a(n)z^n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state to allow memoisation for arbitrary call order */
    static seq_t *A = NULL;      /* array of a(i) values */
    static seq_t *Conv = NULL;   /* array of conv(i) = Σ_{k=0}^{i} a(k)a(i-k) */
    static size_t cap = 0;       /* allocated capacity */
    static size_t last = 0;      /* highest index computed so far */

    /* helper to ensure capacity >= n+1 */
    if (cap == 0) {
        cap = 1;                       /* at least space for a[0] */
        A = (seq_t*)malloc(cap * sizeof(seq_t));
        Conv = (seq_t*)malloc(cap * sizeof(seq_t));
        A[0] = 1;
        Conv[0] = A[0] * A[0];         /* conv(0) = 1*1 */
        last = 0;
    }
    else {
        /* grow on demand, doubling until enough */
        while (cap <= (size_t)n) {
            size_t new_cap = cap * 2;
            A = (seq_t*)realloc(A, new_cap * sizeof(seq_t));
            Conv = (seq_t*)realloc(Conv, new_cap * sizeof(seq_t));
            cap = new_cap;
        }
    }

    /* compute values up to n if necessary */
    if ((size_t)n > last) {
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            seq_t s1 = (i >= 1) ? Conv[i - 1] : 0;
            seq_t s2 = (i >= 2) ? Conv[i - 2] : 0;
            seq_t add = (i >= 2) ? A[i - 2] : 0;
            seq_t val = s1 + s2 + add;
            if (i == 1)               /* subtract 1 for a(1) */
                val -= 1;
            A[i] = val;

            /* compute conv(i) = Σ_{k=0}^{i} A[k]*A[i-k] */
            seq_t sum = 0;
            for (size_t k = 0; k <= i; ++k) {
                sum += A[k] * A[i - k];
            }
            Conv[i] = sum;
        }
        last = (size_t)n;
    }

    return A[(size_t)n];
}
