/*
 * A011614  Legendre symbol (n,157).
 * https://oeis.org/A011614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,157) Computed via Euler's criterion:
 *            a(n) = n^78 (mod 157); Result 0 for multiples of 157, 1 for
 *            quadratic residues, -1 for nonresidues
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9172 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast exponentiation modulo a small prime */
static uint64_t pow_mod_small(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n) {
    uint64_t nmod = (uint64_t)n % 157ULL;
    if (nmod == 0ULL)
        return (seq_t)0;
    uint64_t r = pow_mod_small(nmod, 78ULL, 157ULL);
    if (r == 156ULL)   /* -1 modulo 157 */
        return (seq_t)-1;
    return (seq_t)r;   /* r is 1 */
}
