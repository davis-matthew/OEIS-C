/*
 * A235351  Series reversion of x*(1-3*x-2*x^2)/(1-x).
 * https://oeis.org/A235351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A235351: series reversion of x*(1-3*x-2*x^2)/(1-x)
 *
 * The generating function satisfies 2x^3+3x^2-(1+y)x+y=0 with
 * y = Σ_{n≥1} c_n x^n,  c_1=1, c_2=-2, c_n=-4 (n≥3).
 * From this we obtain the recurrence
 *
 *   S2(n) = Σ_{i=1}^{n-1} a(i)*a(n-i)
 *   a(n)  = 2*Σ_{i=1}^{n-2} a(i)*S2(n-i) + 3*S2(n) - a(n-1)
 *
 *   a(0)=0, a(1)=1.
 *
 * The algorithm stores previously computed values and extends the
 * tables lazily as required.  Complexity is O(n²) arithmetic
 * operations, which is adequate for n up to ~10⁴.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *A = NULL;      /* a[0..] */
    static seq_t *S2 = NULL;    /* S2[0..] */
    static idx_t cached_len = 0; /* number of pre‑computed entries */

    if (n == (idx_t)0)
        return (seq_t)0;         /* a(0) = 0 */

    /* Grow tables if needed */
    if (n >= cached_len) {
        idx_t new_len = n + 1;   /* we need indices 0..n */
        seq_t *tmpA = NULL, *tmpS2 = NULL;

#if defined(__cplusplus)
        tmpA  = (seq_t *)realloc(A,  new_len * sizeof(seq_t));
        tmpS2 = (seq_t *)realloc(S2, new_len * sizeof(seq_t));
#else
        tmpA  = realloc(A,  new_len * sizeof(seq_t));
        tmpS2 = realloc(S2, new_len * sizeof(seq_t));
#endif
        if (!tmpA || !tmpS2) {   /* allocation failure */
            free(tmpA);
            free(tmpS2);
            return (seq_t)0;
        }
        A = tmpA;
        S2 = tmpS2;

        if (cached_len == 0) {
            A[0] = (seq_t)0;
            S2[0] = (seq_t)0;
            if (new_len > 1) {
                A[1] = (seq_t)1;   /* a(1) = 1 */
                S2[1] = (seq_t)0;   /* no pairs summing to 1 */
                cached_len = 2;
            } else {
                cached_len = 1;
            }
        }

        /* Compute new entries from cached_len .. n */
        for (idx_t k = cached_len; k < new_len; ++k) {
            /* S2(k) = Σ_{i=1}^{k-1} a(i)*a(k-i) */
            seq_t sum_S2 = (seq_t)0;
            for (idx_t i = 1; i < k; ++i) {
                sum_S2 += A[i] * A[k - i];
            }
            S2[k] = sum_S2;

            /* S3(k) = Σ_{i=1}^{k-2} a(i)*S2(k-i) */
            seq_t sum_S3 = (seq_t)0;
            if (k >= 3) {
                for (idx_t i = 1; i <= k - 2; ++i) {
                    sum_S3 += A[i] * S2[k - i];
                }
            }

            /* a(k) = 2*sum_S3 + 3*sum_S2 - a(k-1) */
            A[k] = (seq_t)2 * sum_S3 + (seq_t)3 * sum_S2 - A[k - 1];
        }
        cached_len = new_len;
    }

    return A[n];
}
