/*
 * A064179  Infinitary version of Moebius function: infinitary MoebiusMu of n,
 *          equal to mu(n) iff mu(n) differs from zero, else 1 or -1 depending
 *          on whether the sum of the binary digits of the exponents in the
 *          prime decomposition of n is even or odd.
 * https://oeis.org/A064179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = (-1)^{ Σ popcount(e_p) } where the sum runs over the
 *            prime factorization n = Π p^{e_p}. For square‑free n this equals
 *            the usual Möbius μ(n); for non–square‑free n it yields ±1
 *            depending on the parity of the binary digit sum of the exponents
 * Range:     i8 n <= 9498, i16 n <= 9498, i32 n <= 9498, i64 n <= 9498,
 *            i128 n <= 9498, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64() */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but guard anyway */

    /* Prime list arrays: maximum 64 distinct primes for a 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    /* Sum popcount of each exponent.  N ≤ 64 bits → exponent ≤ 64. */
    unsigned popsum = 0;
    for (size_t i = 0; i < cnt; ++i)
        popsum += __builtin_popcountll(exps[i]);

    /* (-1)^{popsum} -> +1 if popsum even; -1 if odd. */
    return (seq_t)((popsum & 1) ? -1 : 1);
}
