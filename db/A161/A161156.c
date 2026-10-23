/*
 * A161156  Positive integers n such that {the number of (non-leading) 0's in
 *          the binary representation of n} is coprime to n, and {the number
 *          of 1's in the binary representation of n} is coprime to n, but
 *          {the number of digits in the binary representation of n} is not
 *          coprime to n.
 * https://oeis.org/A161156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for A161156:
   positive integers n such that
   – the number of non‑leading 0's in binary(n) is coprime to n,
   – the number of 1's in binary(n) is coprime to n,
   – the number of digits in binary(n) is NOT coprime to n.   */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t, seq_t) */

/* popcount of a 64‑bit value */
static inline uint64_t popcount_u64(uint64_t x)
{
    return __builtin_popcountll(x);
}

/* bit length of a 64‑bit value (at least 1) */
static inline uint64_t bitlen_u64(uint64_t x)
{
    return x == 0 ? 1 : 64 - __builtin_clzll(x);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* only defined for n ≥ 1 */

    uint64_t need    = (uint64_t)n;    /* n fits in 64 bits */
    uint64_t found   = 0;
    uint64_t candidate = 0;

    while (found < need)
    {
        ++candidate;                   /* test next integer */

        uint64_t ones  = popcount_u64(candidate);
        uint64_t bits  = bitlen_u64(candidate);
        uint64_t zeros = bits - ones;

        seq_t cand_s   = (seq_t)candidate;
        seq_t ones_s   = (seq_t)ones;
        seq_t bits_s   = (seq_t)bits;
        seq_t zeros_s  = (seq_t)zeros;

        bool zero_coprime;
        if (zeros == 0)                 /* 0 is coprime only with 1 */
            zero_coprime = (candidate == 1);
        else
            zero_coprime = (gcd(zeros_s, cand_s) == (seq_t)1);

        bool one_coprime   = (gcd(ones_s,   cand_s) == (seq_t)1);
        bool bits_coprime  = (gcd(bits_s,   cand_s) == (seq_t)1);

        if (zero_coprime && one_coprime && !bits_coprime)
            ++found;
    }

    return (seq_t)candidate;
}
