/*
 * A128092  a(n) = largest multiple of n which is <= 2^n.
 * https://oeis.org/A128092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - (2^n mod n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 3155 (2026-11-01)
 * Helpers:   pow2, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    /* 2^n as a seq_t value */
    seq_t two_pow = pow2(n);

    /* Remainder of 2^n modulo n */
    seq_t rem = powmod((seq_t)2, n, (seq_t)n);

    /* Largest multiple of n not exceeding 2^n */
    return two_pow - rem;
}
