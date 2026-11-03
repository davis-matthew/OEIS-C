/*
 * A066429  a(n) = 7^n mod n^7.
 * https://oeis.org/A066429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n mod n^7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 493 (2026-11-01)
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
    /* modulus m = n^7 */
    seq_t mod = ipow_idx((seq_t)n, 7);
    /* compute 7^n mod m */
    return powmod((seq_t)7, n, mod);
}
