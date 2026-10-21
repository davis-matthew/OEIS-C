/*
 * A064767  Order of automorphism group of the group C_n X C_n X C_n (where
 *          C_n is the cyclic group of order n).
 * https://oeis.org/A064767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e∥n} p^(9e−6)*(p^3−1)*(p^2−1)*(p−1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 12, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative definition: a(p^e) = p^(9e−6)*(p^3−1)*(p^2−1)*(p−1) */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Factorize n (n fits in uint64_t as per definition). */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t e   = exps[i];
        /* exponent for p^(9e−6) */
        uint64_t exp = 9 * e - 6;

        /* compute term = p^(9e−6) * (p^3 − 1) * (p^2 − 1) * (p − 1) */
        seq_t term = big_pow((seq_t)p, (seq_t)exp);

        seq_t p2 = (seq_t)p * (seq_t)p;          
        seq_t p3 = p2 * (seq_t)p;                    

        term *= (p3 - 1);
        term *= (p2 - 1);
        term *= ((seq_t)p - 1);

        result *= term;
    }

    return result;
}
