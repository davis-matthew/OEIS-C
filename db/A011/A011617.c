/*
 * A011617  Legendre symbol (n,173).
 * https://oeis.org/A011617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,173). Compute as n^86 mod 173, mapping
 *            0→0, 1→1, 172→-1
 * Range:     i8 n <= 1, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 8511 (2026-11-01)
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
    /* Reduce n modulo 173 */
    uint64_t m = n % (uint64_t)173;
    seq_t base = (seq_t)m;

    /* Compute n^86 mod 173 */
    seq_t t = powmod(base, 86ULL, (seq_t)173);

    /* Interpret modular result as Legendre symbol */
    if (t == 0)
        return (seq_t)0;
    else if (t == 1)
        return (seq_t)1;
    else
        return (seq_t)-1;
}
