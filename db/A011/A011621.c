/*
 * A011621  Legendre symbol (n,193).
 * https://oeis.org/A011621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = n^96 (mod 193) (Euler's criterion for Legendre symbol
 *            (n,193)) The function returns 0 for n ≡ 0 (mod 193), +1 for
 *            quadratic residues, and −1 for non‑residues
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* small prime modulus */
    const uint64_t MOD = 193ULL;

    /* Reduce n modulo the prime */
    uint64_t n_mod = (uint64_t)n % MOD;

    /* Fast modular exponentiation: n_mod^96 mod MOD */
    uint64_t result = 1ULL;
    uint64_t base   = n_mod;
    uint64_t exp    = 96ULL;

    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }

    /* Map the residue into the Legendre symbol value: 0, 1, -1 */
    if (result == 0ULL)
        return 0;
    if (result == 1ULL)
        return 1;
    /* result must be 192 == -1 modulo 193 */
    return (seq_t)(-1);
}
