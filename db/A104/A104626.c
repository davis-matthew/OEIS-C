/*
 * A104626  Numbers having three 1's in their base-phi representation.
 * https://oeis.org/A104626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Lucas(2*n-4) + 1 for n > 3 * initial values: a(1)=4,
 *            a(2)=5, a(3)=6
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lucas.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)5;
    if (n == 3) return (seq_t)6;
    idx_t k = 2*n - (idx_t)4;
    return lucas(k) + (seq_t)1;
}
