/*
 * A093949  Expansion of (q^5/12)eta(q^3)^5/eta(q) in powers of q.
 * https://oeis.org/A093949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^{n‑1} in x * Product_{k>0}
 *            (1‑x^{3k})^5/(1‑x^k). Computation: 1. Compute partition numbers
 *            Q[m] (m=0..n‑1) via the pentagonal recurrence. 2. Compute P[m]
 *            (m=0..n‑1), the coefficients of Product_{k>0}
 *            (1‑5x^{3k}+10x^{6k}‑10x^{9k}+5x^{12k}‑x^{15k}), by iteratively
 *            convolving...
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *P_arr = NULL;
static seq_t *Q_arr = NULL;
static seq_t *A_arr = NULL;
static size_t cached_n = 0;         /* number of coefficients stored (equals max n processed) */

static void recompute_up_to(idx_t n)
{
    if (n <= 0) return;
    size_t N = (size_t)n;             /* we need coefficients for indices 0..N‑1 */
    if (cached_n >= N) return;

    /* Reallocate arrays to size N */
    P_arr = (seq_t*)realloc(P_arr, N * sizeof(seq_t));
    Q_arr = (seq_t*)realloc(Q_arr, N * sizeof(seq_t));
    A_arr = (seq_t*)realloc(A_arr, N * sizeof(seq_t));

    /* Initialise to zero */
    memset(P_arr, 0, N * sizeof(seq_t));
    memset(Q_arr, 0, N * sizeof(seq_t));
    memset(A_arr, 0, N * sizeof(seq_t));

    /* ---------- 1. Partition numbers Q[0..N‑1] ---------- */
    if (N > 0) Q_arr[0] = 1;
    for (size_t m = 1; m < N; ++m)
    {
        seq_t sum = 0;
        for (int k = 1; ; ++k)
        {
            size_t g1 = (size_t)k * (3 * k - 1) / 2;
            if (g1 > m) break;
            int sign = (k & 1) ? 1 : -1;
            sum += (seq_t)sign * Q_arr[m - g1];

            size_t g2 = (size_t)k * (3 * k + 1) / 2;
            if (g2 > m) continue;
            sum += (seq_t)sign * Q_arr[m - g2];
        }
        Q_arr[m] = sum;
    }

    /* ---------- 2. Product P[0..N‑1] ---------- */
    P_arr[0] = 1;                     /* start with product = 1 */
    size_t kmax = (N - 1) / 3;         /* largest k such that 3k <= N-1 */

    seq_t *temp = (seq_t*)calloc(N, sizeof(seq_t));

    for (size_t k = 1; k <= kmax; ++k)
    {
        size_t d1 = 3 * k;
        size_t d2 = 6 * k;
        size_t d3 = 9 * k;
        size_t d4 = 12 * k;
        size_t d5 = 15 * k;
        const seq_t coeff[6] = { (seq_t)1, (seq_t)-5, (seq_t)10,
                                 (seq_t)-10, (seq_t)5, (seq_t)-1 };

        /* reset temp */
        memset(temp, 0, N * sizeof(seq_t));

        for (size_t i = 0; i < N; ++i)
        {
            seq_t pv = P_arr[i];
            if (!pv) continue;
            /* coefficient for degree i (d0 = 0) */
            temp[i] += coeff[0] * pv;   /* coeff[0] == 1 */

            if (i + d1 < N) temp[i + d1] += coeff[1] * pv;
            if (i + d2 < N) temp[i + d2] += coeff[2] * pv;
            if (i + d3 < N) temp[i + d3] += coeff[3] * pv;
            if (i + d4 < N) temp[i + d4] += coeff[4] * pv;
            if (i + d5 < N) temp[i + d5] += coeff[5] * pv;
        }
        memcpy(P_arr, temp, N * sizeof(seq_t));
    }
    free(temp);

    /* ---------- 3. Convolution A = P * Q  (coefficients up to N-1) ---------- */
    for (size_t j = 0; j < N; ++j)
    {
        seq_t sum = 0;
        for (size_t i = 0; i <= j; ++i)
            sum += P_arr[i] * Q_arr[j - i];
        A_arr[j] = sum;
    }

    cached_n = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but safe */
    recompute_up_to(n);
    return A_arr[n - 1];
}
