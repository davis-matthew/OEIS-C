/*
 * A169264  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=11*10^{n-1} for 1≤n≤28, and for n≥29 *
 *            a(n)=9·(a(n-1)+a(n-2)+…+a(n-28))−45·a(n-29) (linear recurrence)
 *            * This implements the exact sequence A169264
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;                     /* a(0)=1 */

    /* Allocate array big enough for terms [0..n] */
    idx_t len = n + 1;
    seq_t *v = (seq_t *)malloc(len * sizeof(seq_t));
    if (!v) return 0;                         /* out‑of‑memory: return 0 */

    v[0] = 1;

    /* Small terms (≤28) are given by the simple formula */
    if (n >= 1) {
        v[1] = 11;
        /* compute v[i] = v[i-1] * 10 for i = 2 … min(n,28) */
        idx_t lim = (n < 28) ? n : 28;
        for (idx_t i = 2; i <= lim; ++i) {
            v[i] = v[i - 1] * (seq_t)10;
        }
    }

    /* Recurrence for n ≥ 29 */
    const seq_t nine     = 9;
    const seq_t fortyfive = 45;

    if (n >= 29) {
        for (idx_t i = 29; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 28; ++j) {
                sum += v[i - j];
            }
            v[i] = sum * nine - v[i - 29] * fortyfive;
        }
    }

    seq_t result = v[n];
    free(v);
    return result;
}
