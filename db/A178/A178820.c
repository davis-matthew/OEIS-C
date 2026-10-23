/*
 * A178820  Triangle read by rows: T(n,k) = C(n+3,3) * C(n,k), 0 <= k <= n.
 * https://oeis.org/A178820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = C(r+3,3) * C(r,k) where r is the row containing n
 *            (largest integer with r*(r+1)/2 <= n) and k = n - r*(r+1)/2
 * Range:     u8 n <= 15, u16 n <= 59, u32 n <= 309, u64 n <= 1454,
 *            u128 n <= 5150, big any n
 * Helpers:   binomial_opt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial_opt.h"

seq_t a(idx_t n)
{
    /* Find the row r and the position k within that row. */
    idx_t r = 0;
    idx_t items_before = 0;
    while ((items_before + (r + 1)) <= n)
    {
        items_before += (r + 1);
        r++;
    }
    idx_t k = n - items_before;

    /* Compute C(r+3,3) = (r+3)(r+2)(r+1)/6. */
    seq_t row_coeff = ((seq_t)(r + 3) * (r + 2) * (r + 1)) / (seq_t)6;

    /* Compute C(r,k). */
    seq_t k_coeff = binomial_opt(r, k);

    return row_coeff * k_coeff;
}
