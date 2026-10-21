/*
 * A212770  Expansion of q / (chi(q) * chi(q^2) * chi(q^3) * chi(q^6))^2 in
 *          powers of q where chi() is a Ramanujan theta function.
 * https://oeis.org/A212770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1)=1. * Let e(n)=Σ_{d|n} s(d) where s(d)=+2 for d∈{1,3,8,24},
 *            * s(d)=-2 for d∈{2,4,6,12}. * C(n)=−Σ_{d|n} d · e(d). * Define
 *            f(0)=1 and for k≥1: * f(k)= (1/k) · Σ_{j=1}^{k} C(j)·f(k−j) *
 *            Then a(n)=f(n−1) (n≥1). * The implementation below follows this
 *            recurrence and * uses a safe 128‑bit accumul...
 * Range:     i8 n <= 13, i16 n <= 37, i32 n <= 115, i64 n <= 390,
 *            i128 n <= 1399, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if defined(__cplusplus)
    /* In C++ a big‑integer type may be used; treat accumulator as seq_t. */
    typedef seq_t seq128_t;
#else
    /* In C use the native 128‑bit integer if available. */
    typedef __int128 seq128_t;
#endif

static const idx_t ms[8] = {1, 2, 3, 4, 6, 8, 12, 24};
static const seq_t ss[8] = {(seq_t)  2, (seq_t)-2, (seq_t)  2,
                            (seq_t)-2, (seq_t)-2, (seq_t)  2,
                            (seq_t)-2, (seq_t)  2};

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1 */
    size_t N = (size_t)n;                /* desired term a(n) */
    size_t M = N - 1;                    /* highest f index needed */

    /* Allocate arrays of size M+1 (index 0…M). */
    seq_t *e = (seq_t *)calloc(M + 1, sizeof(seq_t));
    if (!e) return 0;
    seq_t *C = (seq_t *)calloc(M + 1, sizeof(seq_t));
    if (!C) { free(e); return 0; }
    seq_t *tmp = (seq_t *)calloc(M + 1, sizeof(seq_t));
    if (!tmp) { free(e); free(C); return 0; }
    seq_t *f = (seq_t *)calloc(M + 1, sizeof(seq_t));
    if (!f) { free(e); free(C); free(tmp); return 0; }

    /* 1. Compute e[1…M]. */
    for (idx_t idx = 0; idx < 8; ++idx) {
        size_t d = (size_t)ms[idx];
        seq_t val = ss[idx];
        for (size_t j = d; j <= M; j += d)
            e[j] += val;
    }

    /* 2. Compute C[1…M] = -Σ_{d|k} d · e(d). */
    for (size_t d = 1; d <= M; ++d) {
        if (e[d] == 0) continue;
        seq_t contrib = e[d] * (seq_t)d;       /* d * e(d) */
        for (size_t k = d; k <= M; k += d)
            tmp[k] += contrib;
    }
    for (size_t k = 1; k <= M; ++k)
        C[k] = -tmp[k];
    free(tmp);

    /* 3. Compute f[0…M] with recurrence. */
    f[0] = 1;
    for (size_t k = 1; k <= M; ++k) {
        seq128_t sum = 0;
        for (size_t j = 1; j <= k; ++j)
            sum += (seq128_t)C[j] * (seq128_t)f[k - j];
        f[k] = (seq_t)(sum / (seq128_t)k);   /* division is exact */
    }

    seq_t result = f[M];

    /* Free temporary storage. */
    free(e);
    free(C);
    free(f);
    return result;
}
