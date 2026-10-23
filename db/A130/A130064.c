/*
 * A130064  a(n) = (n / SmallestPrimeFactor(n)) * GreatestPrimeFactor(n).
 * https://oeis.org/A130064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n / smallestPrimeFactor(n)) * greatestPrimeFactor(n)
 * Range:     u8 n <= 33, u16 n <= 513, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_gpf.h"

/* Smallest prime factor of n (n >= 2).  Returns n if n is prime. */
static uint64_t _spf(uint64_t n)
{
    if ((n & 1ULL) == 0) return 2ULL;
    for (uint64_t p = 3ULL; p <= n / p; p += 2ULL) {
        if (n % p == 0ULL) return p;
    }
    return n;
}

seq_t a(idx_t n)
{
    if (n <= 1ULL) return (seq_t)1;          /* a(1) = 1 (a(0) unused) */

    uint64_t sp = _spf((uint64_t)n);
    uint64_t gp = _gpf((uint64_t)n);

    seq_t N  = (seq_t)n;
    seq_t res = (N / (seq_t)sp) * (seq_t)gp;
    return res;
}
