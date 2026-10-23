/*
 * A102084  a(1) = 0; for n>0, write 2n=p+q (p, q prime), p*q maximal; then
 *          a(n)=p*q (see A073046).
 * https://oeis.org/A102084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = the maximum product p*q of two primes with p+q = 2n. a(1)
 *            = 0. (For n>1 the largest semiprime whose sum of prime factors
 *            is 2n.)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_simple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_simple.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0. */
    if (n <= 1) return 0;

    /* Work with 64‑bit integers to avoid overflow for n up to about 10^9. */
    uint64_t N   = (uint64_t)n;
    uint64_t sum = N * 2;      /* 2*n */
    uint64_t limit = sum / 2;  /* only need primes up to this point */
    uint64_t best  = 0;

    for (uint64_t p = 2; p <= limit; ++p)
    {
        if (!is_prime_simple(p)) continue;
        uint64_t q = sum - p;
        if (!is_prime_simple(q)) continue;
        uint64_t prod = p * q;
        if (prod > best) best = prod;
    }

    return (seq_t)best;
}
