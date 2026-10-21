/*
 * A079671  Number of irreducible polynomials (over the rationals) of form
 *          a*x^2+b*x+c, 1 <= a,b,c <= n.
 * https://oeis.org/A079671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{a,b,c | 1 ≤ a,b,c ≤ n and b²−4ac is NOT a perfect
 *            square}
 * Range:     u8 n <= 6, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* total number of coefficient triples */
    seq_t total = (seq_t)n * (seq_t)n * (seq_t)n;

    /* If n is zero (outside the defined offset) return zero */
    if (n == 0) return 0;

    /* Maximum possible value of t = (b² - k²)/4 for reducible polynomials */
    idx_t max_t = (n * n) / 4;

    /* Count ordered pairs (a,c) with product t, 1 ≤ a,c ≤ n, 1 ≤ t ≤ max_t */
    uint16_t *cnt = (uint16_t *)malloc((size_t)(max_t + 1) * sizeof(uint16_t));
    if (!cnt) return total;          /* fallback on allocation failure */

    for (size_t i = 0; i <= max_t; ++i) cnt[i] = 0;

    for (idx_t a_val = 1; a_val <= n; ++a_val) {
        for (idx_t c_val = 1; c_val <= n; ++c_val) {
            idx_t prod = a_val * c_val;
            if (prod > max_t) break; /* further c_val only increase prod */
            ++cnt[prod];
        }
    }

    /* Count reducible triples by enumerating possible (b,k) pairs */
    seq_t reducible = 0;
    for (idx_t b = 1; b <= n; ++b) {
        idx_t start_k = (b & 1) ? 1 : 0;           /* k must have same parity as b */
        for (idx_t k = start_k; k < b; k += 2) {
            idx_t t = ((b * b) - (k * k)) >> 2;    /* (b² - k²)/4 */
            if (t <= max_t) {
                reducible += (seq_t)cnt[t];
            }
        }
    }

    free(cnt);
    return total - reducible;
}
