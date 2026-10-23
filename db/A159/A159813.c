/*
 * A159813  Expansion of eta(q) * eta(q^4) * eta(q^14)^4 / (eta(q^2) *
 *          eta(q^7) * eta(q^28)) in powers of q.
 * https://oeis.org/A159813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Expansion of eta(q) * eta(q^4) * eta(q^14)^4 / (eta(q^2) *
 *            eta(q^7) * eta(q^28)) as a q-series with a(1) being the
 *            coefficient of q^1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;        /* offset is 1; undefined for n==0 */
    idx_t N = n;                 /* we need coefficients up to q^(n-1) after shift */

    /* allocate working arrays */
    seq_t *arr  = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    seq_t *tmp  = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!arr || !tmp) return 0;  /* allocation failure safety */

    arr[0] = 1;                  /* start with product = 1 */

    /* ---------- multiply by eta(q) = ∏_{k>=1} (1 - q^k) ---------- */
    for (idx_t k = 1; k <= N; ++k) {
        for (idx_t j = 0; j <= N; ++j) {
            tmp[j] = arr[j];
            if (j >= k) tmp[j] -= arr[j - k];
        }
        seq_t *swap = arr; arr = tmp; tmp = swap;
    }

    /* ---------- multiply by eta(q^4) = ∏_{k>=1} (1 - q^{4k}) ---------- */
    for (idx_t k = 1; k <= N / 4; ++k) {
        idx_t step = 4 * k;
        for (idx_t j = 0; j <= N; ++j) {
            tmp[j] = arr[j];
            if (j >= step) tmp[j] -= arr[j - step];
        }
        seq_t *swap = arr; arr = tmp; tmp = swap;
    }

    /* ---------- multiply by eta(q^14)^4 = ∏_{k>=1} (1 - q^{14k})^4 ---------- */
    for (idx_t k = 1; k <= N / 14; ++k) {
        idx_t s14 = 14 * k;
        idx_t s28 = 28 * k;
        idx_t s42 = 42 * k;
        idx_t s56 = 56 * k;
        for (idx_t j = 0; j <= N; ++j) {
            seq_t v = arr[j];
            if (j >= s14) v -= 4 * arr[j - s14];
            if (j >= s28) v += 6 * arr[j - s28];
            if (j >= s42) v -= 4 * arr[j - s42];
            if (j >= s56) v += 1 * arr[j - s56];
            tmp[j] = v;
        }
        seq_t *swap = arr; arr = tmp; tmp = swap;
    }

    /* ---------- multiply by inverse of eta(q^2) = 1 / ∏_{k>=1} (1 - q^{2k}) ---------- */
    for (idx_t k = 1; k <= N / 2; ++k) {
        idx_t step = 2 * k;
        for (idx_t i = step; i <= N; ++i) {
            arr[i] += arr[i - step];
        }
    }
    /* ---------- inverse of eta(q^7) ---------- */
    for (idx_t k = 1; k <= N / 7; ++k) {
        idx_t step = 7 * k;
        for (idx_t i = step; i <= N; ++i) {
            arr[i] += arr[i - step];
        }
    }
    /* ---------- inverse of eta(q^28) ---------- */
    for (idx_t k = 1; k <= N / 28; ++k) {
        idx_t step = 28 * k;
        for (idx_t i = step; i <= N; ++i) {
            arr[i] += arr[i - step];
        }
    }

    seq_t result = arr[n - 1];   /* shift by the prefactor q^1 */
    free(arr);
    free(tmp);
    return result;
}
