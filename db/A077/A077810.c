/*
 * A077810  Sum of divisors of numbers containing in their decimal
 *          representation only the digits 0 and 1.
 * https://oeis.org/A077810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sigma(A007088(n)) – sum of divisors of the n‑th
 *            binary‑decimal number (decimal digits are the bits of n)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Compute the sum of divisors of a 64‑bit integer.
   Returns the result as seq_t. */
static seq_t sigma_uint64(uint64_t n)
{
    seq_t result = 1;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t term =
            (pow_uint(primes[i], (idx_t)(exps[i] + 1)) - 1) /
            ((seq_t)(primes[i] - 1));
        result *= term;
    }
    return result;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* Convert n to its binary representation interpreted as a
       decimal number consisting only of digits 0 and 1. */
    uint64_t m = (uint64_t)n;
    uint64_t decimal = 0;

    /* Find the number of bits needed. */
    int bits = 0;
    uint64_t temp = m;
    while (temp)
    {
        ++bits;
        temp >>= 1;
    }
    if (bits == 0) bits = 1;   /* n == 0 case – not used in the sequence */

    /* Build the decimal number from MSB to LSB. */
    for (int i = bits - 1; i >= 0; --i)
    {
        uint64_t bit = (m >> i) & 1;
        decimal = decimal * 10 + bit;
    }

    /* Return the sum of divisors of that number. */
    return sigma_uint64(decimal);
}
