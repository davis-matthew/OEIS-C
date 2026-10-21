/*
 * A063086  a(n) = gcd(1 + prime(n+1), 1 + prime(n)).
 * https://oeis.org/A063086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(1 + prime(n+1), 1 + prime(n))
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static uint64_t *primes = NULL;
static size_t prime_count   = 0;
static size_t prime_capacity = 0;

/* Ensure that at least (up_to_index + 1) primes are generated
   (indices 0..up_to_index). */
static void ensure_primes(size_t up_to_index)
{
    if (prime_count > up_to_index) return;

    if (!primes) {
        prime_capacity = 256;
        primes = (uint64_t *)malloc(prime_capacity * sizeof(uint64_t));
        if (!primes) abort();
    }

    /* Initial prime */
    if (prime_count == 0) {
        primes[0] = 2;
        prime_count = 1;
    }

    uint64_t candidate = primes[prime_count - 1] + 1;
    while (prime_count <= up_to_index) {
        /* Test candidate for primality */
        int divisible = 0;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { divisible = 1; break; }
        }
        if (!divisible) {
            /* Store new prime */
            if (prime_count == prime_capacity) {
                prime_capacity *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    prime_capacity * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[prime_count++] = candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1, return 0 */

    /* We need primes up to index n (0‑based) -> prime(n+1) */
    ensure_primes((size_t)n);

    seq_t p1 = (seq_t)primes[(size_t)(n - 1)] + 1; /* prime(n) + 1 */
    seq_t p2 = (seq_t)primes[(size_t)n] + 1;       /* prime(n+1) + 1 */

    return gcd(p1, p2);
}
