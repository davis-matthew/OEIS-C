/*
 * A022459  a(n) = prime(2n+1) mod prime(n).
 * https://oeis.org/A022459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2n+1) mod prime(n)
 * Range:     u8 n <= 26, u16 n <= 4887, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* The OEIS offset is 1: a(1) is the first term of the sequence. */
    if (n == 0) return 0; /* undefined for n < 1, return 0 as a safe fallback */
    seq_t p1 = (seq_t)get_prime(2 * n + 1);  /* prime(2n+1) */
    seq_t p2 = (seq_t)get_prime(n);          /* prime(n)   */
    return p1 % p2;                          /* modulus */
}
