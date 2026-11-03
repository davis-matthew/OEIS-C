/*
 * A200873  Number of 0..n arrays x(0..4) of 5 elements without any interior
 *          element greater than both neighbors or less than both neighbors.
 * https://oeis.org/A200873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{6} p_k * C(n - k + 5, 5) with coefficients p =
 *            {16, -19, 14, -14, 6, -1}
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 183, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Validated: standard, pass, 49 terms through n = 205 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */

    static const int pk[7] = {0, 16, -19, 14, -14, 6, -1};
    seq_t result = 0;

    for (idx_t k = 1; k <= 6; ++k) {
        if (n < k) continue;

        idx_t m   = n - k + 5;            /* argument of the binomial */
        seq_t bin = binomial(m, 5);          /* C(m,5) */

        int coeff = pk[k];
        if (coeff > 0)
            result += mul_small(bin, (unsigned int)coeff);
        else
            result -= mul_small(bin, (unsigned int)(-coeff));
    }

    return result;
}
