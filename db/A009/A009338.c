/*
 * A009338  Expansion of log(1+sin(x)*cos(x)).
 * https://oeis.org/A009338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 0; for n >= 1 * a(n) = U^(n)(0) - Σ_{k=1}^{n-1} a(k) *
 *            U^(n-k)(0) * C(n-1, k-1) * where U(x) = 1 + sin(x) * cos(x) and
 *            * U^(m)(0) = 0 for even m, U^(2k+1)(0) = (-1)^k * 2^{2k}
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 21, i128 n <= 21,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif
#include "../utils/pow2.h"

/* Forward declarations for allocation helpers (if needed) */
#if !defined(__cplusplus)
static void *seq_realloc(void *old, size_t old_cap, size_t new_cap);
static seq_t *alloc_seq_array(size_t n);
#endif

/* Compile-time alias to use C++ vector when compiled as C++ */
#ifdef __cplusplus
static std::vector<seq_t> seq_a;
static std::vector<seq_t> seq_d;
static idx_t computed_max = 0;
static void ensure_capacity(idx_t n) {
    if (n >= static_cast<idx_t>(seq_a.size())) {
        size_t newcap = static_cast<size_t>(n) + 1;
        seq_a.resize(newcap);
        seq_d.resize(newcap);
        if (computed_max == 0) {
            seq_a[0] = (seq_t)0;
            seq_d[0] = (seq_t)1;
        }
    }
}
#else
static seq_t *seq_a = NULL;
static seq_t *seq_d = NULL;
static size_t a_cap = 0;
static idx_t computed_max = 0;
static void ensure_capacity(idx_t n) {
    if (n >= (idx_t)a_cap) {
        size_t newcap = (size_t)n + 1;
        seq_a = (seq_t *)realloc(seq_a, newcap * sizeof(seq_t));
        seq_d = (seq_t *)realloc(seq_d, newcap * sizeof(seq_t));
        if (computed_max == 0) {
            seq_a[0] = (seq_t)0;
            seq_d[0] = (seq_t)1;
        }
        a_cap = newcap;
    }
}
#endif

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    ensure_capacity(n);
    if (n > computed_max) {
        for (idx_t m = computed_max + 1; m <= n; ++m) {
            /* Compute derivative U^(m)(0) */
            seq_t d_m;
            if (m & 1) {
                idx_t k = (m - 1) >> 1;         /* k = (m-1)/2 */
                seq_t val = pow2(2 * k);        /* 2^(2k) */
                if (k & 1) val = -val;          /* sign alternates */
                d_m = val;
            } else {
                d_m = 0;
            }
            seq_d[m] = d_m;

            /* Compute the convolution sum */
            seq_t sum = 0;
            seq_t binom = 1; /* binom(m-1,0) */
            for (idx_t k = 1; k < m; ++k) {
                if (((m - k) & 1) != 0) { /* U^(m-k)(0) non-zero only for odd (m-k) */
                    seq_t term = seq_a[k];
                    term *= seq_d[m - k];
                    term *= binom;
                    sum += term;
                }
                /* Update binomial coefficient for next k: binom(m-1, k) */
                if (k < m - 1) {
                    seq_t mult = (seq_t)(m - k);   /* (m-1 - (k-1)) == m-k */
                    binom = binom * mult / (seq_t)k;
                }
            }
            seq_a[m] = d_m - sum;
        }
        computed_max = n;
    }
    return seq_a[n];
}
