/*
 * A099123  Number of orbits of the wreath product of S_n with S_n on n X n
 *          matrices over {0,1,2,3,4}.
 * https://oeis.org/A099123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(binomial(n + 4, n) + n - 1, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* binomial(n + 4, n) == binomial(n + 4, 4) */
    idx_t n_plus_4 = n + 4;
    seq_t inner = binomial(n_plus_4, 4);

    seq_t sum = inner + (seq_t)(n - 1);
    idx_t sum_idx = (idx_t)sum;      /* fits in idx_t for n ≤ 10000 */

    return binomial(sum_idx, n);
}
