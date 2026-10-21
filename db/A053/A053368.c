/*
 * A053368  a(n) = (5n+2)*C(n) where C(n) = Catalan numbers (A000108).
 * https://oeis.org/A053368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5*n+2)*C(n) where C(n) is the Catalan number A000108
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);
    seq_t factor = (seq_t)5 * (seq_t)n + (seq_t)2;
    return cat * factor;
}
