/*
 * A208238  Smallest prime greater than n, containing it in binary
 *          representation.
 * https://oeis.org/A208238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Find the smallest prime greater than n that contains the binary
 *            representation of n as a contiguous substring
 * Range:     u8 n <= 37, u16 n <= 4179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bin_contains, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/bin_contains.h"

/* External function required by the build. */
seq_t a(idx_t n)
{
    /* Convert the index to a 64‑bit value for calculations. */
    uint64_t N = (uint64_t)n;

    /* Determine bit length of n (treat 0 as length 1). */
    unsigned len_n = (N == 0) ? 1 : (64 - __builtin_clzll(N));

    /* Start searching from the first prime greater than N. */
    uint64_t cand = next_prime(N);   /* provided by utils. */

    /* Find the first prime that contains N’s binary pattern. */
    while (!bin_contains(cand, N, len_n))
        cand = next_prime(cand);

    /* Return the found prime as seq_t. */
    return (seq_t)cand;
}
