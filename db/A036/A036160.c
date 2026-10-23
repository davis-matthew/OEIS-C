/*
 * A036160  a(n) = 3^n mod 223.
 * https://oeis.org/A036160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 223
 * Range:     u8 n <= 47, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    return mod_pow((seq_t)3, n, (seq_t)223);
}
