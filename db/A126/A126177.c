/*
 * A126177  Triangle read by rows: T(n,k) is number of hex trees with n edges
 *          and k leaves (n >= 1, 1 <= k <= 1 + floor(n/2)).
 * https://oeis.org/A126177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(row, k) * where row is the minimal positive integer
 *            with * S(row-1) < n <= S(row), * S(r) = sum_{i=1}^{r} (1 +
 *            floor(i/2)), * k = n - S(row-1), * T(row, k) = 3^(row-2*k+2) *
 *            binomial(2k-2, k-1) * binomial(row, 2k-2) / k * (OEIS A126177,
 *            triangle read by rows)
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find the row containing the nth term */
    idx_t row = 1;
    idx_t cum = 0;   /* cumulative length of preceding rows */
    while (cum + (1 + (row / 2)) < n) {
        cum += 1 + (row / 2);
        ++row;
    }
    idx_t k = n - cum;            /* 1‑based position within the row */

    /* Compute T(row, k) using the closed form */
    idx_t exp = row - (2 * k) + 2; /* exponent is always non‑negative */
    seq_t res = pow3(exp);

    seq_t term1 = binomial(2 * k - 2, k - 1);
    seq_t term2 = binomial(row, 2 * k - 2);

    res *= term1;
    res *= term2;
    res /= k;          /* division is exact */

    return res;
}
