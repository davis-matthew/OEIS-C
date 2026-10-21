/*
 * A080695  Concatenation of the prime power factors (with maximal exponent)
 *          of n; a(1) = 1 by convention.
 * https://oeis.org/A080695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the prime power factors of n (with
 *            maximal exponent) in ascending prime order
 * Range:     u8 n <= 32, u16 n <= 857, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, factor_u64, pow10, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* a(1) = 1 by convention */
    if (n == (idx_t)1)
        return (seq_t)1;

    uint64_t num = (uint64_t)n;

    /* Factorize n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(num, primes, exps, &cnt);

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        /* p^e as a seq_t value */
        seq_t pow_val = pow_uint(primes[i], (idx_t)exps[i]);

        /* Number of decimal digits of p^e */
        int digits = digit_len(pow_val);

        /* 10^digits as seq_t */
        seq_t shift = pow10((idx_t)digits);

        /* Concatenate the value */
        result = result * shift + pow_val;
    }
    return result;
}
