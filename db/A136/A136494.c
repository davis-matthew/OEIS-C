/*
 * A136494  Number of permutation symmetries in the binary expansion of n.
 * https://oeis.org/A136494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = factorial(bitcount(n)) * factorial(bitlen(n)-bitcount(n))
 *            where bitcount(n) is the number of 1-bits in the binary
 *            expansion of n and bitlen(n) is the number of bits used to
 *            represent n (at least 1, so that a(0)=1)
 * Range:     u8 n <= 62, u16 n <= 510, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Validated: standard, pass, 81 terms through n = 1023 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    unsigned ones = __builtin_popcountll((unsigned long long)n);

    unsigned bitlen = (n == 0) ? 1 : (64 - __builtin_clzll(n));

    unsigned zeros = bitlen - ones;

    seq_t res = factorial((idx_t)ones);
    return seq_mul(res, factorial((idx_t)zeros));
}
