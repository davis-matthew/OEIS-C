/*
 * A127349  a(n) = Sum_{i=n..n+1} Sum_{j=i+1..n+2} Sum_{k=j+1..n+3}
 *          prime(i)*prime(j)*prime(k).
 * https://oeis.org/A127349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=n..n+1} Sum_{j=i+1..n+2} Sum_{k=j+1..n+3}
 *            prime(i)*prime(j)*prime(k)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 170, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Retrieve the four consecutive primes starting at index n */
    seq_t p0 = (seq_t)get_prime(n);
    seq_t p1 = (seq_t)get_prime(n + 1);
    seq_t p2 = (seq_t)get_prime(n + 2);
    seq_t p3 = (seq_t)get_prime(n + 3);

    /* Sum of all 3‑prime products among the four */
    return p0 * p1 * p2 + p0 * p1 * p3 + p0 * p2 * p3 + p1 * p2 * p3;
}
