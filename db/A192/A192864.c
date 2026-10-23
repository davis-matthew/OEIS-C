/*
 * A192864  Lower flat primes: odd primes p such that p-1 is a squarefree
 *          number times a power of two.
 * https://oeis.org/A192864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th odd prime p such that (p-1) is a squarefree
 *            number times a power of two. The function searches for the nth
 *            such prime by testing odd candidates for primality and the
 *            flatness condition
 * Range:     u8 n <= 41, u16 n <= 4911, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_flat_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_flat_prime.h"

/* Test whether x is prime (trial division). */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, harmless */
    idx_t count = 0;                      /* how many flat primes found */
    uint64_t cand = 3;                     /* first odd candidate */

    while (count < n) {
        if (is_prime_uint64(cand) && is_flat_prime(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        cand += 2;                         /* next odd number */
    }
    return (seq_t)0;                       /* unreachable for n≥1 */
}
