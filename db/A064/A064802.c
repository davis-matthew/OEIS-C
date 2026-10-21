/*
 * A064802  a(n) = Min { m > n | prime factorizations of m and n differ in one
 *          factor only}, a(1) = 1.
 * https://oeis.org/A064802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest integer m > n such that the prime factorisations of
   m and n differ in exactly one factor; a(1)=1.
   Algorithm: factorise n, for each distinct prime factor p replace one
   occurrence by the next prime q > p, compute product, keep minimal. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/isPrimeTrial.h"          /* declares bool isPrimeTrial(seq_t n); */

static uint64_t next_prime_after(uint64_t p)
{
    /* return the smallest prime strictly greater than p (p ≥ 2) */
    uint64_t cand = (p > 2) ? p + 1 : 3;    /* first candidate > p */
    if (cand % 2 == 0) cand++;               /* make odd when necessary */

    for (;;)
    {
        if (isPrimeTrial((seq_t)cand)) return cand;
        cand += 2;                           /* skip evens */
    }
}

static size_t distinct_prime_factors(uint64_t n, uint64_t *out)
{
    size_t count = 0;
    if (n % 2 == 0) {
        out[count++] = 2;
        while (n % 2 == 0) n /= 2;
    }
    for (uint64_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            out[count++] = d;
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1) out[count++] = n;
    return count;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1)=1 */

    uint64_t n64   = (uint64_t)n;
    uint64_t primes[64];
    size_t pcnt  = distinct_prime_factors(n64, primes);

    uint64_t best = 0;
    bool first   = true;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t q = next_prime_after(p);

        uint64_t base = n64 / p;          /* exact division */
        uint64_t cand64 = base * q;       /* product fits in 64 bits for n≤10 000 */

        if (first || cand64 < best) {
            best = cand64;
            first = false;
        }
    }
    return (seq_t)best;
}
