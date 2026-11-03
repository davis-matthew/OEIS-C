/*
 * A037276  Start with 1; for n>1, replace n with the concatenation of its
 *          prime factors in increasing order.
 * https://oeis.org/A037276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concatenation of the prime factors of n (with
 *            multiplicities) in increasing order
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 2047, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 70 terms through n = 9513 (2026-11-01)
 * Helpers:   factor_u64, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/num_digits_idx.h"

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
    if (n == (idx_t)1)
        return (seq_t)1;

    /* factor n, obtaining primes and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 0;
    bool first = true;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_val = (seq_t)primes[i];
        uint64_t exp = exps[i];
        for (uint64_t k = 0; k < exp; ++k) {
            if (first) {
                result = p_val;
                first = false;
            } else {
                int d = num_digits_idx((idx_t)primes[i]); /* number of decimal digits of the prime */
                result = result * pow10((idx_t)d) + p_val;
            }
        }
    }
    return result;
}
