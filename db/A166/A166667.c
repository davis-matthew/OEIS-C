/*
 * A166667  Totally multiplicative sequence with a(p) = 9p+1 for prime p.
 * https://oeis.org/A166667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} (9*p+1)^e (multiplicative)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by the empty product definition. */
    if (n == 1)
        return (seq_t)1;

    seq_t result = 1;          /* multiplicative accumulation  */
    uint64_t temp   = (uint64_t)n;   /* work on the numeric part only */

    while (temp > 1) {
        uint64_t p   = 2;      /* current factor */
        /* find the smallest prime factor of temp */
        while (p * p <= temp && temp % p != 0) {
            p = (p == 2) ? 3 : p + 2;      /* test only 2, then odd numbers */
        }
        /* if no factor found, temp itself is prime */
        if (p * p > temp)
            p = temp;

        uint64_t exp = 0;                /* multiplicity of prime p */
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }

        seq_t base = 9 * (seq_t)p + 1;   /* 9*p + 1, in seq_t */
        seq_t val  = big_pow(base, (seq_t)exp);  /* (9p+1)^exp */
        result *= val;
    }

    return result;
}
