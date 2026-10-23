/*
 * A011629  Legendre symbol (n,239).
 * https://oeis.org/A011629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Legendre symbol (n,239). By Euler's criterion a(n) =
 *            n^119 (mod 239) -> returns 0 if 239 | n, 1 if the residue is a
 *            quadratic residue mod 239, -1 otherwise
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 239 is prime, (p-1)/2 = 119.  Compute n^119 mod 239. */
    const uint64_t MOD = 239ULL;
    const uint64_t EXP = 119ULL;

    uint64_t base = n % MOD;          /* reduce n mod 239 */
    if (base == 0)
        return (seq_t)0;              /* n divisible by 239 */

    uint64_t exp = EXP;
    uint64_t result = 1 % MOD;

    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    /* result is either 1 or 238 (which represents -1 modulo 239). */
    if (result == 1ULL)
        return (seq_t)1;
    else
        return (seq_t)-1;
}
