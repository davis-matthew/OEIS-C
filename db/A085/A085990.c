/*
 * A085990  Number of topological types of polygons with 2n different sides.
 * https://oeis.org/A085990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (2*n-1)! / 2 (for n > 1), a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for the first entry */
    if (n < 2)
        return (seq_t)0;

    /* Compute (2*n-1)! and divide by 2 at the end.
       We use idx_t for the loop counter to avoid signedness issues. */
    idx_t m = 2*n - 1;          /* m = 2n-1, always positive for n>=2   */
    seq_t prod = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i)
        prod = prod * (seq_t)i;

    /* Since (2n-1) is odd and n>1, the factorial contains at least one factor 2. */
    prod = prod / (seq_t)2;
    return prod;
}
