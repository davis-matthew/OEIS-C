/*
 * A201077  G.f.: 1 / Product_{i>=1}
 *          (1-q^(2*i-1))^2*(1-q^(12*i-8))*(1-q^(12*i-6))*(1-q^(12*i-4))*(1-q^(12*i)).
 * https://oeis.org/A201077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 31, u32 n <= 95, u64 n <= 311,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 988 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A201077
    G.f. = 1 / ∏_{i≥1} (1−q^{2i−1})^2 (1−q^{12i−8}) (1−q^{12i−6}) (1−q^{12i−4}) (1−q^{12i})
    The coefficients a(n) are all positive integers.
    This implementation builds the series by successively multiplying by
    the inverse factors (1−q^m)^{-1} and (1−q^m)^{-2} (the latter is
    applied twice).  Each factor is handled with an in‑place recurrence
    that requires only additions, giving O(n²) time for computing a(n).
*/

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/*   after this, a[i] = old_a[i] + a[i-m] + a[i-2m] + ...   */
static void apply_inv_factor(seq_t *a, size_t n, idx_t m)
{
    size_t step = (size_t)m;
    for (size_t i = step; i <= n; ++i)
        a[i] += a[i - step];
}

/* Auxiliary: multiply the current series by (1 - q^m)^{-1}  */

/*  Entry point required by the harness */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    size_t N = (size_t)n;
    seq_t *coef = mem_new(seq_t, N + 1);
    if (!coef) return (seq_t)0;      /* memory failure: return 0 */

    coef[0] = (seq_t)1;   /* start with 1 */

    /* 1. factors (1 - q^m)^{-2} for odd m : apply twice   */
    for (size_t m = 1; m <= N; m += 2) {
        apply_inv_factor(coef, N, (idx_t)m);
        apply_inv_factor(coef, N, (idx_t)m);
    }

    /* 2. factors (1 - q^m)^{-1} where m = 12i-8,12i-6,12i-4,12i */
    /*   We iterate i until m > N, applying each factor once */

    /* m = 12i - 8 */
    for (idx_t i = 1;; ++i) {
        idx_t m = (idx_t)(12 * i - 8);
        if (m > n) break;
        apply_inv_factor(coef, N, m);
    }
    /* m = 12i - 6 */
    for (idx_t i = 1;; ++i) {
        idx_t m = (idx_t)(12 * i - 6);
        if (m > n) break;
        apply_inv_factor(coef, N, m);
    }
    /* m = 12i - 4 */
    for (idx_t i = 1;; ++i) {
        idx_t m = (idx_t)(12 * i - 4);
        if (m > n) break;
        apply_inv_factor(coef, N, m);
    }
    /* m = 12i      */
    for (idx_t i = 1;; ++i) {
        idx_t m = (idx_t)(12 * i);
        if (m > n) break;
        apply_inv_factor(coef, N, m);
    }

    seq_t result = coef[N];
    mem_free(coef);
    return result;
}
