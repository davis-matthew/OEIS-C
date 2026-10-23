/*
 * A035838  Coordination sequence for A_12 lattice.
 * https://oeis.org/A035838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{d=1..12} C(13,d) * C(n-1,d-1) * C(12-d+n,n), with
 *            a(0)=1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 72, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    for (idx_t d = 1; d <= 12; ++d) {
        seq_t c13 = binomial(13, d);            /* C(13,d) */
        seq_t c1  = binomial(n - 1, d - 1);      /* C(n-1,d-1) */
        if (c1 == 0)                             /* skip when comb = 0 */
            continue;
        seq_t c2 = binomial(12 - d + n, n);      /* C(12-d+n,n) */
        sum += c13 * c1 * c2;
    }
    return sum;
}
