/*
 * A167020  a(n) = 1 iff 6n-1 is prime.
 * https://oeis.org/A167020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff 6n‑1 is prime
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 6n−1 fits in idx_t for all expected n */
    idx_t candidate = 6ULL * n - 1ULL;
    return is_prime(candidate) ? 1 : 0;
}
