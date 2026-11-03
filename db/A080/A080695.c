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
 * Validated: standard, pass, 80 terms through n = 9755 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
        seq_t pow_val = ipow_u64_idx(primes[i], (idx_t)exps[i]);

        /* Number of decimal digits of p^e */
        int digits = num_digits(pow_val);

        /* 10^digits as seq_t */
        seq_t shift = pow10((idx_t)digits);

        /* Concatenate the value */
        result = result * shift + pow_val;
    }
    return result;
}
