/*
 * A028312  Odd numbers k such that {1..k-1} cannot be partitioned into
 *          disjoint sets I, J such that 2I == -J (mod k) and I, J are unions
 *          of cyclotomic cosets mod k.
 * https://oeis.org/A028312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = the n-th odd integer having a prime factor ≡ 3 (mod 8)
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* Simple sieve generating primes up to LIMIT */
static void build_primes(uint32_t LIMIT)
{
    bool *sieve = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!sieve) return;
    for (uint32_t i = 2; i <= LIMIT; ++i) sieve[i] = true;
    for (uint32_t p = 2; p * p <= LIMIT; ++p)
        if (sieve[p])
            for (uint32_t j = p * p; j <= LIMIT; j += p) sieve[j] = false;

    prime_count = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) if (sieve[i]) ++prime_count;

    prime_list = (uint32_t *)malloc(prime_count * sizeof(uint32_t));
    if (!prime_list) { free(sieve); return; }

    size_t idx = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) if (sieve[i]) prime_list[idx++] = i;

    free(sieve);
}

/* Check if x has a prime divisor ≡ 3 (mod 8) */
static bool has_prime_3mod8(seq_t x)
{
    if (x < 3) return false;
    seq_t tmp = x;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = prime_list[i];
        if ((seq_t)p * (seq_t)p > tmp) break;
        if (tmp % p == 0) {
            while (tmp % p == 0) tmp /= p;
            if (p & 7U) {            /* p % 8 == 3 : low 3 bits are 011 */
                if ((p & 7U) == 3U) return true;
            }
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        if ((tmp & 7U) == 3U) return true;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!prime_list) build_primes(100000);   /* enough for required range */
    if (n == 0) return 0;   /* undefined, but just in case */

    idx_t count = 0;
    seq_t val = 1;          /* start from odd 1 */
    while (count < n) {
        val += 2;           /* only odd candidates */
        if (has_prime_3mod8(val))
            ++count;
    }
    return val;
}
