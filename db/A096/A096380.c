/*
 * A096380  Differences between the sum of the first three primes and the
 *          fourth prime in consecutive prime quadruples.
 * https://oeis.org/A096380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + prime(n+1) + prime(n+2) - prime(n+3)
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* n is 1‑based as per OEIS offset */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 1);
    seq_t p3 = (seq_t)get_prime(n + 2);
    seq_t p4 = (seq_t)get_prime(n + 3);
    return p1 + p2 + p3 - p4;
}
