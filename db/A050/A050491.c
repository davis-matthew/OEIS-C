/*
 * A050491  a(n) = C(n)*(12n+1) where C(n) = Catalan numbers (A000108).
 * https://oeis.org/A050491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Catalan(n) * (12*n + 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t c   = catalan(n);
    seq_t mul = (seq_t)(12 * n + 1);
    return c * mul;
}
