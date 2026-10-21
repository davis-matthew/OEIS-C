/*
 * A058671  McKay-Thompson series of class 42A for Monster.
 * https://oeis.org/A058671
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in A + 2 + 1/A , where * A = ((η(q)
 *            η(q^6) η(q^14) η(q^21))/(η(q^2) η(q^3) η(q^7) η(q^42)))^2
 * Range:     u8 n <= 22, u16 n <= 64, u32 n <= 200, u64 n <= 679,
 *            u128 n <= 1000, big any n
 * Helpers:   multiply_by_one_minus_qm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/multiply_by_one_minus_qm.h"

/* ----  Cached series data ---- */
static seq_t *num_series = NULL;       /* numerator product  */
static seq_t *den_series = NULL;       /* denominator product  */
static seq_t *den_inv_series = NULL;   /* inverse of denominator  */
static seq_t *s_series = NULL;         /* S(q) = N/D  */
static seq_t *s_inv_series = NULL;     /* inverse of S  */
static seq_t *s2_series = NULL;        /* S^2  */
static seq_t *s_inv_sq_series = NULL;  /* S^-2  */
static size_t current_cap = 0;

/* ----  Recompute all series up to N  ---- */
static void recompute_all_up_to(size_t N)
{
    size_t needed = N + 1;            /* we need indices 0..N inclusive */

    /* Allocate new arrays */
    seq_t *tmp1 = (seq_t *)malloc(needed * sizeof(seq_t));
    seq_t *tmp2 = (seq_t *)malloc(needed * sizeof(seq_t));
    seq_t *tmp3 = (seq_t *)malloc(needed * sizeof(seq_t));
    seq_t *tmp4 = (seq_t *)malloc(needed * sizeof(seq_t));
    seq_t *tmp5 = (seq_t *)malloc(needed * sizeof(seq_t));
    seq_t *tmp6 = (seq_t *)malloc(needed * sizeof(seq_t));
    seq_t *tmp7 = (seq_t *)malloc(needed * sizeof(seq_t));

    if (!tmp1 || !tmp2 || !tmp3 || !tmp4 || !tmp5 || !tmp6 || !tmp7)
        return; /* allocation failure; caller will handle */

    /* Initialise to zero */
    memset(tmp1, 0, needed * sizeof(seq_t));
    memset(tmp2, 0, needed * sizeof(seq_t));
    memset(tmp3, 0, needed * sizeof(seq_t));
    memset(tmp4, 0, needed * sizeof(seq_t));
    memset(tmp5, 0, needed * sizeof(seq_t));
    memset(tmp6, 0, needed * sizeof(seq_t));
    memset(tmp7, 0, needed * sizeof(seq_t));

    /* Set constant terms */
    tmp1[0] = 1;  /* num series starts with 1 */
    tmp2[0] = 1;  /* den series starts with 1 */

    /* ----- Build numerator product:  (1 - q^k)(1 - q^(6k))(1 - q^(14k))(1 - q^(21k)) ---- */
    const int numer_doms[] = {1, 6, 14, 21};
    for (int d_idx = 0; d_idx < 4; ++d_idx) {
        int d = numer_doms[d_idx];
        for (int k = 1; (long long)k * d <= (long long)N; ++k) {
            int m = k * d;
            multiply_by_one_minus_qm(tmp1, needed, m);
        }
    }

    /* ----- Build denominator product:  (1 - q^(2k))(1 - q^(3k))(1 - q^(7k))(1 - q^(42k)) ---- */
    const int denom_doms[] = {2, 3, 7, 42};
    for (int d_idx = 0; d_idx < 4; ++d_idx) {
        int d = denom_doms[d_idx];
        for (int k = 1; (long long)k * d <= (long long)N; ++k) {
            int m = k * d;
            multiply_by_one_minus_qm(tmp2, needed, m);
        }
    }

    /* ----- Inverse of denominator series:  D_inv  ---- */
    tmp3[0] = 1;
    for (size_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= n; ++k)
            sum += tmp2[k] * tmp3[n - k];
        tmp3[n] = -sum;
    }

    /* ----- Compute S = N * D_inv  ---- */
    for (size_t n = 0; n <= N; ++n) {
        seq_t sum = 0;
        for (size_t k = 0; k <= n; ++k)
            sum += tmp1[k] * tmp3[n - k];
        tmp4[n] = sum;
    }

    /* ----- Inverse of S  ---- */
    tmp5[0] = 1;
    for (size_t n = 1; n <= N; ++n) {
        seq_t sum = 0;
        for (size_t k = 1; k <= n; ++k)
            sum += tmp4[k] * tmp5[n - k];
        tmp5[n] = -sum;
    }

    /* ----- Compute S^2  ---- */
    for (size_t n = 0; n <= N; ++n) {
        seq_t sum = 0;
        for (size_t k = 0; k <= n; ++k)
            sum += tmp4[k] * tmp4[n - k];
        tmp6[n] = sum;
    }

    /* ----- Compute (S^-1)^2  ---- */
    for (size_t n = 0; n <= N; ++n) {
        seq_t sum = 0;
        for (size_t k = 0; k <= n; ++k)
            sum += tmp5[k] * tmp5[n - k];
        tmp7[n] = sum;
    }

    /* Free old arrays */
    free(num_series);
    free(den_series);
    free(den_inv_series);
    free(s_series);
    free(s_inv_series);
    free(s2_series);
    free(s_inv_sq_series);

    /* Assign new arrays */
    num_series      = tmp1;
    den_series      = tmp2;
    den_inv_series  = tmp3;
    s_series        = tmp4;
    s_inv_series    = tmp5;
    s2_series       = tmp6;
    s_inv_sq_series = tmp7;

    current_cap = N;
}

/* ----  Main sequence function ---- */
seq_t a(idx_t n)
{
    if (n == -1) return 1; /* offset term */

    /* Ensure series are long enough for this n */
    size_t need = (size_t)n + 1;          /* we need S^2 up to n+1 and S^-2 up to n-1 */
    if (need > current_cap)
        recompute_all_up_to(need);

    seq_t result = 0;

    /* Contribution from A = S^2 shifted by -1 */
    if ((size_t)n + 1 <= current_cap)
        result += s2_series[(size_t)n + 1];

    /* Constant 2 only at n == 0 */
    if (n == 0)
        result += 2;

    /* Contribution from 1/A = q * S^-2 */
    if (n >= 1 && (size_t)n - 1 <= current_cap)
        result += s_inv_sq_series[(size_t)n - 1];

    return result;
}
