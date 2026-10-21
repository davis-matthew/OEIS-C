/*
 * A094070  a(n) = A000085(n) * A000110(n).
 * https://oeis.org/A094070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000085(n+1) * A000110(n+1) where A000085 is the
 *            involution (fixed-point-free) numbers and A000110 is the Bell
 *            numbers. The function computes these two sequences up to n+1
 *            using the simple recurrences: I(0)=1, I(1)=1,
 *            I(k)=I(k-1)+(k-1)*I(k-2) B(0)=1, B(i)=Σ_{k=0}^{i-1}
 *            binom(i-1,k)*B(k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: Uses only integer arithmetic and the standard library. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    idx_t N = n + 1;                 /* we need terms up to n+1 */
    /* Allocate space for involution and Bell numbers */
    seq_t *I = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *B = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!I || !B)
        return (seq_t)0;              /* out of memory: return 0 */

    /* ---- Involution numbers I(k) ---- */
    I[0] = (seq_t)1;
    if (N >= 1)
        I[1] = (seq_t)1;
    for (idx_t k = 2; k <= N; ++k) {
        I[k] = I[k - 1] + (seq_t)(k - 1) * I[k - 2];
    }

    /* ---- Bell numbers B(i) ---- */
    B[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;                       /* binom(i-1,0) */
        for (idx_t k = 0; k < i; ++k) {
            sum += bin * B[k];
            if (k + 1 < i) {                        /* update binom */
                bin = bin * (seq_t)(i - 1 - k) /
                      (seq_t)(k + 1);
            }
        }
        B[i] = sum;
    }

    seq_t result = I[N] * B[N];

    free(I);
    free(B);
    return result;
}
