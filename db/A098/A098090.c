/*
 * A098090  Numbers k such that 2k-3 is prime.
 * https://oeis.org/A098090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (p_{n+1} + 3) / 2, where p_{n+1} is the (n+1)-st prime *
 *            (prime(1) = 2). We use the helper get_prime() to obtain the
 *            required * prime
 * Range:     u8 n <= 53, u16 n <= 7060, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* n >= 1 must hold for valid calls */
    seq_t p = (seq_t)get_prime(n + (idx_t)1);   /* (n+1)-th prime, 1-indexed */
    return (p + (seq_t)3) / (seq_t)2;
}
