/*
 * A011606  Legendre symbol (n,109).
 * https://oeis.org/A011606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,109) = n^54 (mod 109)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0, a(n)∈{0,1,-1} for all n≥0 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is idx_t (uint64_t) */
    /* If n is a multiple of 109, the Legendre symbol is 0 */
    if (n % 109ULL == 0ULL)
        return (seq_t)0;

    /* Reduce n modulo 109 */
    uint64_t r = n % 109ULL;
    const uint64_t mod = 109ULL;
    uint64_t result = 1ULL;
    uint64_t base = r % mod;
    uint64_t exp = 54ULL;

    /* Fast exponentiation (mod 109) */
    while (exp > 0ULL) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }

    /* result is either 1 or 108 (= -1 mod 109) */
    return (result == 1ULL) ? (seq_t)1 : (seq_t)-1;
}
