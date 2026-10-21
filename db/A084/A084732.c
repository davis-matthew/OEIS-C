/*
 * A084732  Let p be the n-th prime; Cp(x) be the p-th cyclotomic polynomial
 *          (x^p-1)/(x-1); a(n) is the first prime Cp(x) after Cp(1).
 * https://oeis.org/A084732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A085399(prime(n))
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Obtain the n-th prime p. */
    uint32_t p = get_prime(n);            /* primes are 32‑bit */

    seq_t result, pow_xp, sum;
    idx_t x = 2;

    for (;; ++x)
    {
        /* Compute x^p. */
        pow_xp = big_pow((seq_t)x, (seq_t)p);

        /* Compute C_p(x) = (x^p-1)/(x-1). */
        sum = pow_xp - (seq_t)1;
        if (x != 2)
            sum /= (seq_t)(x - 1);

        if (is_prime_big(sum))
        {
            result = sum;
            break;
        }
    }

    return result;
}
