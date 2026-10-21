/*
 * A092520  Number of square divisors of n-th cube: a(n) = A046951(n^3).
 * https://oeis.org/A092520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} floor((3*e+2)/2) (number of square divisors of
 *            n³)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor.h"

seq_t a(idx_t n)
{
    /* prime factors and their exponents of the integer n (up to 64 bits) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    /* factor n: primes[], exps[], cnt */
    factor(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e   = exps[i];                       /* exponent of prime p in n */
        uint64_t term = (3 * e + 2) / 2;              /* floor((3*e+2)/2) */
        result *= (seq_t)term;                        /* multiplicative */
    }
    return result;
}
