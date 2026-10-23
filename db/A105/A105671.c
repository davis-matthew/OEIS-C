/*
 * A105671  a(2n) = Lucas(2n+3)^2, a(2n+1) = Lucas(2n+1)^2.
 * https://oeis.org/A105671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2n) = Lucas(2n+3)^2, a(2n+1) = Lucas(2n+1)^2
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 21, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   lucas
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lucas.h"

seq_t a(idx_t n)
{
    seq_t l;
    if ((n & 1) == 0) {           /* even index: 2k */
        l = lucas(n + 3);
    } else {                      /* odd index: 2k+1 */
        l = lucas(n);
    }
    return l * l;
}
