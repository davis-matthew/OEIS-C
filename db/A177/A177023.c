/*
 * A177023  a(n) = 2^(2*n) mod (2*n+1).
 * https://oeis.org/A177023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n mod (2*n+1)
 * Range:     u8 n <= 128, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 103 terms through n = 9899 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    /* Compute modulus (2*n+1) using seq_t arithmetic to avoid truncation */
    seq_t mod = (seq_t)n * 2 + 1;
    /* Compute 4^n modulo mod using efficient modular exponentiation */
    return powmod((seq_t)4, n, mod);
}
