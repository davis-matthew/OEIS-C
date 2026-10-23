/*
 * A013897  a(n) = 20^(5*n + 4).
 * https://oeis.org/A013897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20^(5*n + 4)
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    idx_t exp = 5 * n + 4;
    return seq_pow((seq_t)20, exp);
}
