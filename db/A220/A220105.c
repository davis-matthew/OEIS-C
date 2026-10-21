/*
 * A220105  2^(n-1) mod n^2.
 * https://oeis.org/A220105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) mod n^2
 * Range:     u8 n <= 15, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)n * (seq_t)n;
    return mod_pow((seq_t)2, n - 1, mod);
}
