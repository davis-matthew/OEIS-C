/*
 * A205105  Ordered differences of numbers 3^j-2^j, as in A001047.
 * https://oeis.org/A205105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = [3^j - 2^j] - [3^i - 2^i] where j is the smallest integer
 *            with j*(j-1)/2 ≥ n, and i = n - (j-2)*(j-1)/2. (1 ≤ i < j)
 * Range:     u8 n <= 10, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* find j such that T_j = (j-1)*j/2 ≥ n */
    idx_t j = 2;
    while ( (j - 1) * j / 2 < n )
        j++;

    /* number of terms before the j‑th block */
    idx_t prev = (j - 2) * (j - 1) / 2;
    /* position of i within that block (1‑based) */
    idx_t i = n - prev;

    /* compute the required powers of 2 and 3 */
    seq_t p3_j = pow3(j);
    seq_t p3_i = pow3(i);
    seq_t p2_j = pow2(j);
    seq_t p2_i = pow2(i);

    /* a(n) = (3^j - 3^i) - (2^j - 2^i) */
    seq_t result = p3_j - p3_i - p2_j + p2_i;
    return result;
}
