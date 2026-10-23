/*
 * A033760  Expansion of Product_{d | 48} theta_3(q^d).
 * https://oeis.org/A033760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{d|48} θ₃(q^d) = number of
 *            integer representations of n as Σ_{d|48} d·k_d² with k_d∈ℤ,
 *            counting signs separately
 * Range:     u8 n <= 20, u16 n <= 95, u32 n <= 1485, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for partial convolution sums. */
    seq_t *res = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!res) return (seq_t)0;
    res[0] = 1;

    /* Divisors of 48. */
    static const idx_t divisors[] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 48};
    const size_t divcnt = sizeof(divisors) / sizeof(divisors[0]);

    for (size_t di = 0; di < divcnt; ++di) {
        idx_t d = divisors[di];
        seq_t *next = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!next) { free(res); return (seq_t)0; }

        for (idx_t k = 0; k <= n; ++k) {
            seq_t r = res[k];
            if (!r) continue;
            for (idx_t s = 0;; ++s) {
                idx_t add = d * s * s;
                idx_t idx = k + add;
                if (idx > n) break;
                if (s == 0)
                    next[idx] += r;
                else
                    next[idx] += r * 2;
            }
        }
        free(res);
        res = next;
    }

    seq_t ans = res[n];
    free(res);
    return ans;
}
