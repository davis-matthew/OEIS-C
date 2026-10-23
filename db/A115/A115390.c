/*
 * A115390  Binomial transform of tribonacci sequence A000073.
 * https://oeis.org/A115390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0..n} C(n,k) * tribonacci(k) (binomial transform of
 *            A000073)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Returns the n-th term of A115390 (binomial transform of the tribonacci sequence). */
seq_t a(idx_t n) {
    /* Allocate space for tribonacci values up to n */
    size_t len = (size_t)n + 1;
    seq_t *T = (seq_t*)malloc(len * sizeof(seq_t));
    if (!T) return 0; /* fallback on allocation failure */

    /* base tribonacci values: T(0)=0, T(1)=0, T(2)=1 */
    if (len > 0) T[0] = 0;
    if (len > 1) T[1] = 0;
    if (len > 2) T[2] = 1;
    for (idx_t i = 3; i <= n; ++i) {
        T[i] = T[i-1] + T[i-2] + T[i-3];
    }

    /* Compute the binomial transform */
    seq_t result = 0;
    seq_t C = 1; /* C(n,0) */
    for (idx_t k = 0; k <= n; ++k) {
        result += C * T[k];
        if (k < n) {
            /* Update binomial coefficient: C = C * (n-k) / (k+1) */
            C = C * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    free(T);
    return result;
}
