/*
 * A202842  Number of secondary structures of size n having no stacks of
 *          length 2.
 * https://oeis.org/A202842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the functional equation G = 1 + z G + f G
 *            (G-1)/(1+f) with f = z^2(1 - z^2 + z^4)/(1 - z^2). The
 *            coefficients a(n) can be computed by the following recurrence:
 *            a(0) = 1 a(n) = sum_{k=1..n} B_k a(n-k) + sum_{k=2..n} f_k
 *            (G^2(n-k)) + f_n where B_1 = 1, B_k = (k>=2)? f_{k-1} − 2 f_k
 *            :...
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    size_t N = (size_t)n;
    seq_t *a_arr = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *g2    = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *f     = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!a_arr || !g2 || !f) return (seq_t)0;   // memory error

    a_arr[0] = (seq_t)1;   /* a(0) */
    g2[0]    = (seq_t)1;   /* G^2(0) = a0*a0 */

    /* build f: f[2] = 1, f[6] = f[8] = ... = 1 */
    if (n >= 2) f[2] = (seq_t)1;
    for (idx_t m = 6; m <= n; m += 2) f[m] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t posSum = 0, negSum = 0;

        /* B-contributions */
        for (idx_t j = 1; j <= i; ++j) {
            seq_t aj = a_arr[i - j];
            int B;
            if (j == 1) {
                B = 1;                     /* B_1 = 1 */
            } else {
                B = (int)f[j - 1] - 2 * (int)f[j];
            }
            if (B > 0)     posSum += (seq_t)B * aj;
            else if (B < 0) negSum += (seq_t)(-B) * aj;
        }

        /* sumA = Σ_{k=2..i} f_k * G^2(i-k)  (f_k is 0 or 1) */
        seq_t sumA = 0;
        if (i >= 2) sumA += g2[i - 2];
        for (idx_t k = 6; k <= i; k += 2) sumA += g2[i - k];

        seq_t C = f[i];            /* constant term f_i */

        /* Combine parts: a(i) = posSum + sumA + C - negSum */
        seq_t total = posSum + sumA + C;
        total -= negSum;
        a_arr[i] = total;

        /* Update G^2 for future use */
        for (idx_t t = 0; t < i; ++t) {
            idx_t idx = i + t;
            if (idx > n) continue;
            seq_t prod = a_arr[i] * a_arr[t];
            g2[idx] += prod + prod;          /* 2 * prod     */
        }
        idx_t idx = i + i;
        if (idx <= n) {
            seq_t prod = a_arr[i] * a_arr[i];
            g2[idx] += prod;
        }
    }

    seq_t result = a_arr[n];
    free(a_arr);
    free(g2);
    free(f);
    return result;
}
