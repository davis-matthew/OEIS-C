/*
 * A050489  a(n) = C(n)*(10*n + 1) where C(n) = Catalan numbers (A000108).
 * https://oeis.org/A050489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = catalan(n) * (10*n + 1)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A050489 (OEIS A050489) */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t mult = (((seq_t)n) * 10) + 1;
    seq_t cat  = catalan(n);
    return cat * mult;
}
