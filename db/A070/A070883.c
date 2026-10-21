/*
 * A070883  Bitwise XOR of n and n-th prime.
 * https://oeis.org/A070883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = XOR(n, p_n), where p_n is the n-th prime
 * Range:     u8 n <= 54, u16 n <= 6542, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided get_prime helper and the XOR operator. */
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    seq_t p = get_prime(n);          /* n-th prime */
    return (seq_t)n ^ p;             /* bitwise XOR with n */
}
