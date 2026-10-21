/*
 * A227553  Number of solutions to x^2 - y^2 - z^2 == 1 (mod n).
 * https://oeis.org/A227553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^s ‖ n} f(p,s) where f(2,1) = 4 f(2,s>1)= 2^(2s-1)
 *            f(p,s) = (p+1)*p^(2s-1) if p≡1 (mod 4) f(p,s) = (p-1)*p^(2s-1)
 *            if p≡3 (mod 4)
 * Range:     u8 n <= 16, u16 n <= 249, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* arrays large enough for all prime factors of n up to 64 bits */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];
        seq_t contrib;

        if (p == 2) {
            if (exp == 1)
                contrib = (seq_t)4;
            else
                contrib = big_pow((seq_t)2, (seq_t)(2 * exp - 1));
        } else {
            seq_t pseq = (seq_t)p;
            seq_t pw   = big_pow(pseq, (seq_t)(2 * exp - 1));
            if (p % 4 == 1)
                contrib = (pseq + (seq_t)1) * pw;
            else
                contrib = (pseq - (seq_t)1) * pw;
        }
        result *= contrib;
    }

    return result;
}
