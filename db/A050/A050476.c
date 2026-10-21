/*
 * A050476  a(n) = C(n)*(6*n + 1) where C(n) = Catalan numbers (A000108).
 * https://oeis.org/A050476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n)*(6*n + 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t cat = catalan(n);
    seq_t mult = (seq_t)6 * (seq_t)n + 1;
    return cat * mult;
}
