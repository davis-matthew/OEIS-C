/*
 * A048865  a(n) is the number of primes in the reduced residue system mod n.
 * https://oeis.org/A048865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = pi(n) - A001221(n) * where pi(n) is the number of primes
 *            <= n and A001221(n) * is the number of distinct prime factors of
 *            n. * This equals the number of primes <= n that are coprime to n
 * Range:     u8 n <= 1620, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9837 (2026-11-01)
 * Helpers:   omega, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/omega.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* For n < 2 the answer is 0 (a(1)=0). */
    if (n < 2)
        return (seq_t)0;

    /* Compute pi(n) using the provided helper. */
    seq_t pi = prime_pi(n);

    /* Count distinct prime factors of n.  The helper returns int. */
    int d = omega((uint64_t)n);

    /* pi(n) >= d, the result is always non‑negative. */
    return pi - (seq_t)d;
}
