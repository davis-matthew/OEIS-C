/*
 * A188250  Number of arrangements of 4 nonzero numbers x(i) in -n..n with the
 *          sum of x(i)*x(i+1) equal to zero.
 * https://oeis.org/A188250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (x1,x2,x3,x4) with x_i ∈ {±1,…,±n},
 *            * non‑zero, satisfying x1*x2 + x2*x3 + x3*x4 = 0
 * Range:     u8 n <= 5, u16 n <= 49, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   count_k_range, gcd_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/count_k_range.h"
#include "../utils/gcd_idx_t.h"

/* Euclidean GCD for unsigned 64‑bit integers. */

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, a(0) unused */

    /* Pre‑compute floor(n / i) for all i. */
    idx_t *floor_n = (idx_t *)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!floor_n) return 0;        /* allocation failure, return 0 */
    for (idx_t i = 1; i <= n; ++i) floor_n[i] = n / i;

    seq_t result = 0;

    for (idx_t b = 1; b <= n; ++b) {
        for (idx_t c = 1; c <= n; ++c) {
            idx_t g   = gcd_idx_t(b, c);
            idx_t b1  = b / g;
            idx_t c1  = c / g;
            idx_t N1  = floor_n[c1];
            idx_t N2  = floor_n[b1];

            seq_t cnt_plus  = count_k_range(N1, N2, g, +1);
            seq_t cnt_minus = count_k_range(N1, N2, g, -1);
            seq_t cnt_total = 2 * (cnt_plus + cnt_minus);  /* 2 for B sign */

            result += cnt_total;
        }
    }

    free(floor_n);
    return result;
}
