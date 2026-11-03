/*
 * A011626  Legendre symbol (n,227).
 * https://oeis.org/A011626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = LegendreSymbol(n, 227) = n^113 mod 227 (Euler's
 *            criterion)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9315 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute a^113 (mod 227) using exponentiation by squaring. */
static inline uint64_t mod_pow_227(uint64_t a)
{
    const uint64_t mod = 227ULL;
    uint64_t result = 1ULL % mod;
    uint64_t base   = a % mod;
    uint64_t exp    = 113ULL;          /* (227-1)/2 */
    while (exp != 0ULL) {
        if (exp & 1ULL) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    const uint64_t mod = 227ULL;
    uint64_t n_mod = n % mod;
    if (n_mod == 0ULL)             /* LegendreSymbol(0,227) = 0 */
        return (seq_t)0;
    uint64_t val = mod_pow_227(n_mod);
    if (val == 1ULL)               /* quadratic residue */
        return (seq_t)1;
    /* val == 226ULL == mod-1 → quadratic nonresidue */
    return (seq_t)-1;
}
