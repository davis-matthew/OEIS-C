/*
 * A220862  a(n) = smallest m>0 such that the n-th rational prime p splits in
 *          the imaginary quadratic extension field K = Q(sqrt(-m)).
 * https://oeis.org/A220862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = smallest positive integer m such that the n‑th prime p
 *            splits in the imaginary quadratic field K = Q(sqrt(-m)). For
 *            n>=2 this equals -A088192(n); the first term is the exceptional
 *            -7
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required utilities */
#include <stdint.h>
#include "../utils/get_prime.h"

/* the function to implement */
seq_t a(idx_t n)
{
    /* special first term */
    if (n == 1) return (seq_t)(-7);

    /* get the n-th prime (p >= 3) */
    uint32_t p = get_prime(n);

    /* if p ≡ 1 (mod 4) then m = 1 is the minimal solution */
    if ((p & 3u) == 1u) return (seq_t)(-1);

    /* p ≡ 3 (mod 4): find the largest quadratic residue modulo p,
       then m = p – that residue. */
    uint32_t limit = (p - 1u) >> 1u;   /* iterate a = 1..(p-1)/2 */
    uint64_t p64 = (uint64_t)p;
    uint32_t max_sqr = 0;

    for (uint32_t a = 1; a <= limit; ++a) {
        uint64_t sq = (uint64_t)a * a % p64;
        if (sq > max_sqr) max_sqr = (uint32_t)sq;
    }

    uint32_t m = p - max_sqr;
    return -(seq_t)m;
}
