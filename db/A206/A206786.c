/*
 * A206786  Remainder of n^340 divided by 341.
 * https://oeis.org/A206786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^340 mod 341
 * Range:     u8 n <= 1, u16 n <= 341, u32 n <= 341, u64 n <= 341,
 *            u128 n <= 341, big any n
 * Validated: standard, pass, 83 terms through n = 333 (2026-11-01)
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
    /* Reduce the base modulo 341 before exponentiation to keep numbers small. */
    seq_t base = (seq_t)n % (seq_t)341;
    return powmod(base, 340, (seq_t)341);
}
