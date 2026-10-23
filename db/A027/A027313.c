/*
 * A027313  a(n) = Sum_{k=0..2n} (k+1) * A026323(n, 2n-k).
 * https://oeis.org/A027313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=0, 6 for n=1, otherwise 3^(n+1) (for n>1)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)6;
    return pow3(n + 1);
}
