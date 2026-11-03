/*
 * A138959  a(n) = 8^n mod 5^n.
 * https://oeis.org/A138959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n mod 5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 23 (2026-11-01)
 * Helpers:   ipow_idx, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ipow_idx.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    /* Compute modulus 5^n */
    seq_t mod = ipow_idx((seq_t)5, n);

    /* Compute 8^n mod 5^n */
    seq_t result = powmod((seq_t)8, n, mod);

    return result;
}
