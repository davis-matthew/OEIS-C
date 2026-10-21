/*
 * A237830  Number of partitions of n such that (greatest part) - (least part)
 *          < number of parts.
 * https://oeis.org/A237830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of xⁿ in
 *            (1/∏(1‑xᵏ))·(x/(1‑x))·∑ₖ₌₁^∞(-1)^(k‑1)·x^(3k(k‑1)/2)·(1‑x^(2k))
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 134, u64 n <= 427,
 *            u128 n <= 1479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* size of arrays */
    size_t N = (size_t)n + 1;

    /* compute partition numbers p[0..n] by the standard coin‑change DP */
    seq_t *p = (seq_t*)malloc(N * sizeof(seq_t));
    if (!p) return 0;              /* in case of allocation failure */
    memset(p, 0, N * sizeof(seq_t));
    p[0] = 1;
    for (size_t part = 1; part <= n; ++part) {
        for (size_t sum = part; sum <= n; ++sum) {
            p[sum] += p[sum - part];
        }
    }

    /* compute coefficients s[0..n] of the factor (x/(1‑x))*∑(-1)^(k‑1)(x^(3k(k‑1)/2)-x^(3k(k‑1)/2+2k)) */
    int *s = (int*)calloc(N, sizeof(int));
    if (!s) { free(p); return 0; }

    for (size_t k = 1; ; ++k) {
        size_t gk = 3 * k * (k - 1) / 2;
        if (gk + 1 > n) break;
        int sign = (k % 2 == 1) ? +1 : -1;
        size_t m = gk + 1;
        for (; m <= n; ++m) s[m] += sign;
        size_t m2 = gk + 2 * k + 1;
        if (m2 <= n) {
            for (m = m2; m <= n; ++m) s[m] -= sign;
        }
    }

    /* convolution p * s to obtain a(n) */
    seq_t posSum = 0;
    seq_t negSum = 0;
    for (size_t i = 0; i <= n; ++i) {
        int coeff = s[n - i];
        if (coeff == 0) continue;
        seq_t mult = p[i] * (seq_t)(coeff > 0 ? coeff : -coeff);
        if (coeff > 0) posSum += mult;
        else           negSum += mult;
    }
    seq_t result = posSum - negSum;

    free(p);
    free(s);
    return result;
}
