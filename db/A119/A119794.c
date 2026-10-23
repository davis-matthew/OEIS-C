/*
 * A119794  a(n) is the product of the positive integers each of which is <= n
 *          and is divisible by exactly one prime dividing n (but is coprime
 *          to every other prime dividing n). (a(1) = 1).
 * https://oeis.org/A119794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 21, u64 n <= 33, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): product of all positive integers ≤ n divisible by
 * exactly one prime divisor of n and coprime to the other prime
 * divisors of n.  The calculation is performed by brute‑force
 * iteration over multiples of each prime factor of n. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n <= 1u) return 1;

    /* Work with 64‑bit arithmetic for simplicity */
    uint64_t m = (uint64_t)n;

    /* Extract the distinct prime factors of m */
    uint64_t primes[64];
    size_t   pc = 0;
    uint64_t t   = m;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            primes[pc++] = p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) primes[pc++] = t;  /* any remaining prime factor */

    /* Accumulate the product */
    seq_t prod = 1;

    for (size_t i = 0; i < pc; ++i) {
        uint64_t prime_i = primes[i];
        for (uint64_t k = 1;; ++k) {
            uint64_t val = prime_i * k;
            if (val > m) break;

            /* Check that val is not divisible by any other prime factor of m */
            bool ok = true;
            for (size_t j = 0; j < pc; ++j) {
                if (j == i) continue;
                if (k % primes[j] == 0) { ok = false; break; }
            }

            if (ok) prod *= (seq_t)val;
        }
    }
    return prod;
}
