/*
 * A159312  Logarithm derivative of the g.f. of A159311 such that a(n) =
 *          (n-1)*A159311(n) + 1.
 * https://oeis.org/A159312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A159312:  a(n) = (n-1)*f(n)+1 where f(0)=1 and
 * f(n) = 1 + Σ_{k=1}^{n-1} a(k)*f(n-k).
 */

#include <stdlib.h>   /* for malloc, realloc, free */

static seq_t *A = NULL;    /* a[1..max_n]  (a[0] unused) */
static seq_t *F = NULL;    /* f[0..max_n]  */
static idx_t  max_n = 0;   /* highest n for which a[n] is stored */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset 1 – n<1 is undefined */
        return (seq_t)0;

    /* grow cache if necessary */
    if (n > max_n) {
        idx_t newSize = n + 1;          /* indices 0..n needed */

        seq_t *aNew = (seq_t *)realloc(A, newSize * sizeof(seq_t));
        seq_t *fNew = (seq_t *)realloc(F, newSize * sizeof(seq_t));
        if (!aNew || !fNew) {           /* allocation failed – abort */
            return (seq_t)0;
        }
        A = aNew;
        F = fNew;

        /* ensure f[0]=1; already set after first allocation */
        if (max_n == 0) {
            F[0] = (seq_t)1;
        }

        /* compute new terms up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            seq_t sum = 0;               /* Σ_{k=1}^{i-1} a[k]*f[i-k] */
            for (idx_t k = 1; k < i; ++k)
                sum += A[k] * F[i - k];

            F[i] = sum + (seq_t)1;          /* f(i) = sum + 1 */
            A[i] = ((seq_t)(i - 1)) * F[i] + (seq_t)1;   /* a(i) = (i-1)*f + 1 */
        }
        max_n = n;
    }
    return A[n];
}
