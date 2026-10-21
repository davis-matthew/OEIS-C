/*
 * A204985  Ordered differences of numbers 2^k for k>=1.
 * https://oeis.org/A204985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 2^i - 2^j, where i, j satisfy i > j >= 1 and i*(i-1)/2 >=
 *            n > (i-1)*(i-2)/2. For any n >= 1, find i as the smallest
 *            integer with i*(i-1)/2 >= n, then set j = n - (i-1)*(i-2)/2. The
 *            result is computed in seq_t using the provided pow2 utility
 * Range:     u8 n <= 28, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row index i (corresponding to the larger exponent) */
    idx_t i = 2;
    while ((i * (i - 1)) / 2 < n) {
        ++i;
    }

    /* Compute column index j (the smaller exponent) */
    idx_t j = n - ((i - 1) * (i - 2)) / 2;

    /* Compute the difference 2^i - 2^j */
    seq_t val = pow2(i) - pow2(j);
    return val;
}
