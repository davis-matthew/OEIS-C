/*
 * A129810  9^(9^9) mod n.
 * https://oeis.org/A129810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^(9^9) mod n
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* exponent 9^9 = 387420489 */
    const uint64_t EXP = 387420489ULL;
    return mod_pow((seq_t)9, EXP, (seq_t)n);
}
