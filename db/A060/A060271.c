/*
 * A060271  Difference between smallest prime following and largest prime
 *          preceding 2*(n-th prime).
 * https://oeis.org/A060271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = next_prime(2*prime(n)) – prev_prime(2*prime(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computes the difference between the smallest prime following
   and the largest prime preceding 2 times the n‑th prime. */

#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* obtain the n‑th prime (1‑indexed) */
    uint64_t p = (uint64_t)get_prime(n);

    /* 2*prime(n) is even and >2, so it is never prime */
    uint64_t two_p = p * 2ULL;

    /* primes immediately before and after two_p */
    uint64_t prev = prev_prime(two_p);
    uint64_t next = next_prime(two_p);

    /* difference fits in seq_t for all realistic n */
    return (seq_t)(next - prev);
}
