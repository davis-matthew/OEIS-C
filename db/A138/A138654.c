/*
 * A138654  7^n mod 4^n.
 * https://oeis.org/A138654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n mod 4^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   pow2, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    /* 4^n = 2^(2n) */
    idx_t exp2 = n << 1;
    seq_t mod = pow2(exp2);
    seq_t base = (seq_t)7;
    return powmod(base, n, mod);
}
