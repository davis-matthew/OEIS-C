/*
 * A095072  Primes in whose binary expansion the number of 0-bits is one more
 *          than the number of 1-bits.
 * https://oeis.org/A095072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th prime p such that in the binary expansion of p
 *            the number of 0‑bits is exactly one more than the number of
 *            1‑bits
 * Range:     u8 n <= 4, u16 n <= 400, u32 n <= 9742, u64 n <= 9742,
 *            u128 n <= 9742, big any n
 * Helpers:   bitlen_64, popcount_64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_64.h"
#include "../utils/popcount_64.h"

/* 64‑bit deterministic Miller–Rabin primality test */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    /* Bases sufficient for 64‑bit integers */
    const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, p = a % n, tmp = d;
        while (tmp) {
            if (tmp & 1) x = (x * p) % n;
            p = (p * p) % n;
            tmp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Test whether a 64‑bit integer satisfies the bit‑balance property */
static bool satisfies_property(uint64_t x)
{
    if (x < 2) return false;
    unsigned int ones = popcount_64(x);
    unsigned int bits = bitlen_64(x);
    unsigned int zeros = bits - ones;
    return zeros == ones + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for 0, return 0 for safety */

    static uint64_t *primes = NULL;
    static size_t size = 0;       /* number of primes stored */
    static size_t capacity = 0;   /* allocated capacity */

    /* Ensure the cache is large enough for n entries */
    while (size < (size_t)n) {
        uint64_t candidate = (size == 0) ? 3 : primes[size - 1] + 2;
        while (true) {
            if (is_prime_64(candidate) && satisfies_property(candidate)) {
                /* store it */
                if (size == capacity) {
                    capacity = (capacity == 0) ? 16 : capacity * 2;
                    primes = (uint64_t*)realloc(primes, capacity * sizeof(uint64_t));
                }
                primes[size++] = candidate;
                break;
            }
            candidate += 2;   /* only odd numbers */
        }
    }
    return (seq_t)primes[n - 1];
}
