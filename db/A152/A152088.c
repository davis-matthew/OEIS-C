/*
 * A152088  Positive integers k that when written in binary have exactly the
 *          same number of (non-leading) 0's as the number of divisors of k.
 * https://oeis.org/A152088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   The n-th term of A152088 is the n-th positive integer k such
 *            that the number of non‑leading zeros in the binary
 *            representation of k equals the number of positive divisors of k
 * Range:     u8 n <= 22, u16 n <= 4778, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary functions for divisor counting and floor(log2). */
#include "../utils/divisor_count_u64.h"
#include "../utils/floor_log2_uint64.h"
#include <stdint.h>

/* Return the n-th term of A152088 (1‑indexed). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* Undefined for n < 1, but harmless. */

    idx_t count = 0;
    uint64_t k = 1;  /* Candidate integer. */

    while (count < n)
    {
        /* Number of bits needed to represent k. */
        unsigned bits = floor_log2_uint64(k) + 1;
        /* Number of 1‑bits in k. */
        unsigned ones = __builtin_popcountll(k);
        /* Non‑leading zeros in the binary representation. */
        unsigned zeros = bits - ones;

        /* Number of divisors of k. */
        unsigned divcnt = divisor_count_u64(k);

        if (zeros == divcnt)
        {
            ++count;
            if (count == n)
                return (seq_t)k;  /* Found the required term. */
        }
        ++k;
    }

    /* Unreachable, but keep compiler happy. */
    return (seq_t)0;
}
