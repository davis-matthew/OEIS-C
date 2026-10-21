/*
 * A218255  Next prime after 10*n.
 * https://oeis.org/A218255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least prime greater than 10*n
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/next_prime.h"

seq_t a(idx_t n) {
    /* Compute 10*n as a 64‑bit value; the tests use n up to 10^4, so no overflow */
    uint64_t start = (uint64_t)n * 10ULL;
    uint64_t p = next_prime(start);
    return (seq_t)p;
}
