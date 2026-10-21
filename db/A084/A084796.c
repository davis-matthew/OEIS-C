/*
 * A084796  Replace n with concatenation of its prime factors in decreasing
 *          order.
 * https://oeis.org/A084796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the concatenation of the prime factors of n in
 *            decreasing order. For n=1 the value is 1
 * Range:     u8 n <= 11, u16 n <= 63, u32 n <= 2047, u64 n <= 9488,
 *            u128 n <= 9488, big any n
 * Helpers:   digit_count, factor_u64, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

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
        seq_t mult = pow10(digit_count(p)); /* 10^digit_count(p) as seq_t */
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
