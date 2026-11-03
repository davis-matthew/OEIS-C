/*
 * A011595  Legendre symbol (n,61).
 * https://oeis.org/A011595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,61) = n^30 (mod 61) represented as 0,
 *            1, or -1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9378 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute n^30 modulo 61, then map 60 to -1. */
static seq_t legendre_mod61(idx_t n) {
    /* n modulo 61 fits in uint64_t */
    uint64_t base = (uint64_t)(n % 61u);
    uint64_t res  = 1;
    uint64_t exp  = 30;

    while (exp) {
        if (exp & 1) res = (res * base) % 61u;
        base = (base * base) % 61u;
        exp >>= 1;
    }

    if (res == 0u) return (seq_t)0;
    if (res == 1u) return (seq_t)1;
    return (seq_t)-1;   /* res == 60u -> -1 */
}

seq_t a(idx_t n) {
    return legendre_mod61(n);
}
