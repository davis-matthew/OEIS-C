/*
 * A072055  a(n) = 2*prime(n)+1.
 * https://oeis.org/A072055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*prime(n)+1
 * Range:     u8 n <= 31, u16 n <= 3512, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Fetch the n-th prime (1‑based). */
    uint32_t p = get_prime(n);
    /* Compute 2*prime(n)+1 in seq_t. */
    return ((seq_t)2 * (seq_t)p) + 1;
}
