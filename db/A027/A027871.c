/*
 * A027871  a(n) = Product_{i=1..n} (3^i - 1).
 * https://oeis.org/A027871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prod_{i=1..n} (3^i - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t term = pow3(i);
        term = term - 1;
        result = result * term;
    }
    return result;
}
