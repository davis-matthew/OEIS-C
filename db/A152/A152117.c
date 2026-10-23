/*
 * A152117  a(n) = n*(n-th prime) + (n+1)*((n+1)-th prime).
 * https://oeis.org/A152117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n-th prime) + (n+1)*((n+1)-th prime)
 * Range:     u8 n <= 6, u16 n <= 79, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* n ≥ 1; the get_prime function uses 1‑based indexing (get_prime(1)=2). */
    seq_t p1 = (seq_t)get_prime(n);
    seq_t p2 = (seq_t)get_prime(n + 1);
    return (seq_t)n * p1 + (seq_t)(n + 1) * p2;
}
