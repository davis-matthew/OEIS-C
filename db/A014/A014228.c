/*
 * A014228  Product of 3 successive Catalan numbers.
 * https://oeis.org/A014228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) * Catalan(n+1) * Catalan(n+2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t c0 = catalan(n);
    seq_t c1 = catalan(n + 1);
    seq_t c2 = catalan(n + 2);
    return c0 * c1 * c2;
}
