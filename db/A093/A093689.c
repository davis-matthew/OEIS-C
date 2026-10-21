/*
 * A093689  Least k such that prime(n) divides A007406(k), the numerator of
 *          the k-th generalized harmonic number H(k,2) = Sum_{i=1..k} 1/i^2.
 * https://oeis.org/A093689
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n):  the least k such that prime(n) divides the numerator of
   the k‑th generalized harmonic number H(k,2) = Σ_{i=1..k} 1/i².
   The algorithm sums i⁻² modulo p = prime(n); the first k for which the
   cumulative sum is 0 mod p is returned.  For primes p > 3 the answer
   is always ≤ (p–1)/2, so the loop terminates at that bound.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* p is the n‑th prime (n ≥ 3).  We only need its 32‑bit value. */
    uint32_t p32 = get_prime(n);
    uint64_t p = (uint64_t)p32;

    /* Upper bound for k: (p‑1)/2.  The result never exceeds this. */
    uint32_t half = (uint32_t)((p32 - 1) / 2);

    /* Allocate a temporary array of modular inverses, reused across calls. */
    static uint32_t *inv = NULL;
    static size_t inv_cap = 0;
    if (inv_cap < (size_t)p32) {
        free(inv);
        inv = (uint32_t *)malloc((size_t)p32 * sizeof(uint32_t));
        inv_cap = (size_t)p32;
    }

    /* sum = Σ_{i=1..k} inv(i)²  (mod p) */
    uint64_t sum = 0;

    inv[1] = 1U;                       /* 1⁻¹ ≡ 1 (mod p) */
    for (uint32_t i = 1; i <= half; ++i) {
        if (i > 1) {                   /* compute inv[i] using the linear method */
            uint64_t hi = p / i;       /* p / i  (integer division) */
            uint32_t lo = (uint32_t)(p % i); /* p % i  < i ≤ half ≤ p-1   */
            /* inv[i] = (p – hi * inv[lo]) mod p  */
            uint64_t t = hi * inv[lo];
            uint32_t r = (uint32_t)(t % p);
            inv[i] = (p32 - r) % p32;  /* may be 0 when r == 0   */
        }

        /* square the inverse and add to cumulative sum */
        uint64_t sq = ((uint64_t)inv[i] * inv[i]) % p;
        sum += sq;
        if (sum >= p) sum -= p;
        if (sum == 0) {
            return (seq_t)i;          /* first k with zero sum */
        }
    }

    /* For p > 3 the sum cannot be zero before k = half, so return it. */
    return (seq_t)half;
}
