/*
 * A053507  a(n) = binomial(n-1,2)*n^(n-3).
 * https://oeis.org/A053507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n-1,2) * n^(n-3)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    /* binomial(n-1,2) = (n-1)*(n-2)/2 */
    seq_t bin = ((seq_t)(n-1) * (n-2)) / 2;

    /* n^(n-3) */
    seq_t pw = pow_uint((uint64_t)n, n-3);

    return bin * pw;
}
