/*
 * A084796  Replace n with concatenation of its prime factors in decreasing
 *          order.
 * https://oeis.org/A084796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the concatenation of the prime factors of n in
 *            decreasing order. For n=1 the value is 1
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 2047, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 74 terms through n = 9488 (2026-11-01)
 * Helpers:   factor_u64, num_digits0
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/num_digits0.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Concatenate prime factors of n in decreasing order. */
static seq_t concat_factors(uint64_t n)
{
    if (n <= 1) /* a(1) = 1 (and a(0) is unused) */
        return 1;

    /* Arrays large enough for all possible prime factors of a 64‑bit integer. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 0;
    for (size_t i = cnt; i-- > 0; ) {
        uint64_t p = primes[i];
        unsigned int e = (unsigned int)exps[i];
        seq_t mult = pow10(num_digits0(p)); /* 10^digit_count(p) as seq_t */
        for (unsigned int k = 0; k < e; ++k) {
            result = result * mult + (seq_t)p;
        }
    }
    return result;
}

seq_t a(idx_t n)
{
    return concat_factors((uint64_t)n);
}
