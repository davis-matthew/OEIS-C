/*
 * A203514  Vandermonde sequence using x^2 + xy + y^2 applied to
 *          (1,3,5,...,2n-1).
 * https://oeis.org/A203514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∏_{1≤i<j≤n} ((2i−1)^2 + (2i−1)(2j−1) + (2j−1)^2)
 *               = ∏_{1≤i<j≤n} ( (2i−1)^2 + (2i−1)(2j−1) + (2j−1)^2 )
 */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;          /* a(1) = 1, no pairs to multiply */

    seq_t product = (seq_t)1;
    /* compute all pairs (i,j) with 1 ≤ i < j ≤ n */
    for (idx_t i = 1; i < n; ++i) {
        seq_t xi = (seq_t)(2 * i - 1);
        for (idx_t j = i + 1; j <= n; ++j) {
            seq_t xj = (seq_t)(2 * j - 1);
            seq_t term = xi * xi + xi * xj + xj * xj;
            product *= term;
        }
    }
    return product;
}
