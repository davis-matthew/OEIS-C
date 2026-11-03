/*
 * A138955  a(n) = 8^n mod 3^n.
 * https://oeis.org/A138955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n mod 3^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   ipow_idx, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t mod = ipow_idx((seq_t)3, n);
    return powmod((seq_t)8, n, mod);
}
