/*
 * A011615  Legendre symbol (n,163).
 * https://oeis.org/A011615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) == n^81 (mod 163) Map result: 0 -> 0, 1 -> 1, 162 -> -1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce n modulo 163 */
    uint64_t base = n % 163LLU;
    uint64_t exp  = 81ULL;
    uint64_t mod  = 163ULL;
    uint64_t result = 1ULL;

    /* fast modular exponentiation */
    while (exp > 0) {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }

    /* Map modular result to Legendre symbol */
    if (result == 0)
        return (seq_t)0;
    else if (result == 1)
        return (seq_t)1;
    else /* result == 162 for non‑residue */
        return (seq_t)-1;
}
