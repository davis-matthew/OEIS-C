/*
 * A036126  a(n) = 3^n mod 43.
 * https://oeis.org/A036126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n mod 43
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    return mod_pow((seq_t)3, n, (seq_t)43);
}
