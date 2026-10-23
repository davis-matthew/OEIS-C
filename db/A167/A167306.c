/*
 * A167306  Totally multiplicative sequence with a(p) = 5*(p+2) for prime p.
 * https://oeis.org/A167306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   totally multiplicative sequence with a(p) = 5*(p+2) for all
 *            primes p. The general term is a(n) = Π(5*(p+2))^e for each prime
 *            factor p^e of n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t tmp = n;            /* work on a 64‑bit copy of n */
    seq_t result = (seq_t)1;

    /* factor 2 separately */
    unsigned e = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++e;
    }
    if (e > 0) {
        seq_t base = (seq_t)(5 * ((uint64_t)2 + 2));
        seq_t pow_val = (seq_t)1;
        for (unsigned i = 0; i < e; ++i) pow_val *= base;
        result *= pow_val;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            unsigned mul = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++mul;
            }
            seq_t base = (seq_t)(5 * (p + 2));
            seq_t pow_val = (seq_t)1;
            for (unsigned i = 0; i < mul; ++i) pow_val *= base;
            result *= pow_val;
        }
    }

    /* remaining prime > sqrt(original n) */
    if (tmp > 1) {
        seq_t base = (seq_t)(5 * (tmp + 2));
        result *= base;          /* exponent is 1 */
    }

    return result;
}
