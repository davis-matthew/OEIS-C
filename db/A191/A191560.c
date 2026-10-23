/*
 * A191560  a(n) = 25^(4*n+3).
 * https://oeis.org/A191560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25^(4*n+3)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 2, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t exp = ((seq_t)n) * 4 + 3;
    return big_pow((seq_t)25, exp);
}
