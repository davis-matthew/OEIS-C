/*
 * A165481  Legendre symbol (n,28657).
 * https://oeis.org/A165481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 28657, i32 n <= 28657, i64 n <= 28657,
 *            i128 n <= 28657, big any n
 * Validated: standard, pass, 98 terms through n = 7572 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Legendre symbol (n,28657) */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    const uint64_t p = 28657ULL;          /* the prime 28657 */
    const seq_t mod = (seq_t)p;           /* modulus as seq_t */
    const uint64_t exp = (p-1)/2;         /* Euler's criterion exponent */

    seq_t base = (seq_t)n % mod;          /* reduce n modulo p */
    seq_t r = powmod(base, exp, mod);    /* compute n^((p-1)/2) mod p */

    if (r == (seq_t)0)          return (seq_t)0;   /* divisible by p */
    if (r == (seq_t)1)          return (seq_t)1;   /* quadratic residue */
    /* otherwise r == p-1 => Legendre symbol is -1 */
    return (seq_t)-1;
}
