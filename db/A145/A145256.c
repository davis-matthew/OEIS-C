/*
 * A145256  a(n) = the smallest integer > n that is non-coprime to n and has
 *          the same number of 1's in its binary representation as n has.
 * https://oeis.org/A145256
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest integer > n that is non-coprime to n and has
 *            the same number of 1's in its binary representation as n has
 * Range:     u8 n <= 127, u16 n <= 9127, u32 n <= 9127, u64 n <= 9127,
 *            u128 n <= 9127, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/* Count the number of set bits (1's) in an integer of type seq_t. */
static uint32_t popcount_seq_t(seq_t x)
{
    uint32_t count = 0;
    while (x) {
        count += (uint32_t)(x & 1);
        x >>= 1;
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n < 2) {
        /* For indices below the offset the OEIS site does not care.
           Returning 0 (or any value) is acceptable. */
        return (seq_t)0;
    }

    seq_t n_seq = (seq_t)n;
    uint32_t target_bits = popcount_seq_t(n_seq);

    seq_t start = n_seq + 1;
    seq_t limit = (seq_t)2 * n_seq;  /* Since a(n) <= 2n */

    for (seq_t m = start; m <= limit; ++m) {
        if (popcount_seq_t(m) != target_bits)
            continue;
        if (gcd(m, n_seq) > 1)
            return m;
    }

    /* Should never reach here because 2n is always a valid answer. */
    return limit;
}
