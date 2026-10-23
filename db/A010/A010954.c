/*
 * A010954  Binomial coefficient C(38,n).
 * https://oeis.org/A010954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(38,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n > 38u)
        return 0;                 /* C(38,n) = 0 for n > 38 */

    idx_t k = n;
    if (k > 38u - k)
        k = 38u - k;              /* use symmetry */

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
        result = result * (seq_t)(38u - k + i) / i;

    return result;
}
