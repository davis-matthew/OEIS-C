/*
 * A220105  2^(n-1) mod n^2.
 * https://oeis.org/A220105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) mod n^2
 * Range:     u8 n <= 15, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9062 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t mod = (seq_t)n * (seq_t)n;
    return powmod((seq_t)2, n - 1, mod);
}
