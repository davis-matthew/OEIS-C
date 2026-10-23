/*
 * A132973  Expansion of psi(-q)^3 / psi(-q^3) in powers of q where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A132973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n≥1: n*a(n) = –Σ_{k=1}^{n} (Σ_{d|k} d*c_d) * a(n‑k),
 *            where c_k = 3 + 3*I(4|k)+I(6|k) – 3*I(2|k) – I(3|k) – I(12|k)
 * Range:     i8 n <= 25, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t N = (size_t)n + 1;

    /* c[k] for 1..n   */
    int *c = (int *)malloc(N * sizeof(int));
    /* alpha[k] = Σ_{d|k} d*c[d]  */
    seq_t *alpha = (seq_t *)calloc(N, sizeof(seq_t));
    /* aseq[k] = a(k)  */
    seq_t *aseq = (seq_t *)malloc(N * sizeof(seq_t));

    if (!c || !alpha || !aseq) {
        free(c);
        free(alpha);
        free(aseq);
        return (seq_t)1;   /* fallback on allocation failure */
    }

    /* compute c[k] */
    for (idx_t k = 1; k <= n; ++k) {
        int val = 3;
        if (k % 4 == 0) val += 3;
        if (k % 6 == 0) val += 1;
        if (k % 2 == 0) val -= 3;
        if (k % 3 == 0) val -= 1;
        if (k % 12 == 0) val -= 1;
        c[(size_t)k] = val;
    }

    /* compute alpha[k] */
    for (idx_t d = 1; d <= n; ++d) {
        int cd = c[(size_t)d];
        if (cd == 0)
            continue;
        seq_t add = (seq_t)d * (seq_t)cd;
        for (idx_t m = d; m <= n; m += d) {
            alpha[(size_t)m] += add;
        }
    }

    aseq[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k) {
            sum += alpha[(size_t)k] * aseq[(size_t)(i - k)];
        }
        seq_t val = sum / (seq_t)i;   /* exact division */
        aseq[(size_t)i] = -val;
    }

    seq_t result = aseq[(size_t)n];

    free(c);
    free(alpha);
    free(aseq);

    return result;
}
