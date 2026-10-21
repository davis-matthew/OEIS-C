/*
 * A091396  a(n) = Product_{ p | n } (1 + Legendre(2,p) ).
 * https://oeis.org/A091396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p | n} (1 + Legendre(2,p)) multiplicative: a(2^e) = 1,
 *            for odd prime p: a(p^e) = 0 if p ≡ 3 or 5 (mod 8) a(p^e) = 2 if
 *            p ≡ 1 or 7 (mod 8)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* ----- prime table utilities ------------------------------------------------ */
static uint32_t *prime_table = NULL;
static size_t      prime_count  = 0;
static uint32_t    sieve_max    = 0;

/* Extend the prime table so that all primes ≤ limit are stored. */
static void extend_primes_to(uint32_t limit)
{
    if (limit <= sieve_max)
        return;

    if (!prime_table) {
        /* initial allocation */
        prime_table = (uint32_t*)malloc(sizeof(uint32_t) * 16);
        if (!prime_table) abort();
        prime_count = 0;
    }

    for (uint32_t cand = (sieve_max < 2 ? 2 : sieve_max + 1); cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_table[i];
            if ((uint64_t)p * p > cand)
                break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* grow table if needed */
            if (prime_count == 0 || prime_count % 16 == 0) {
                size_t new_size = prime_count ? prime_count * 2 : 16;
                uint32_t *tmp = (uint32_t*)realloc(prime_table, new_size * sizeof(uint32_t));
                if (!tmp) abort();
                prime_table = tmp;
            }
            prime_table[prime_count++] = cand;
        }
    }
    sieve_max = limit;
}

/* ----- main OEIS sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    /* special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t x = (uint64_t)n;

    /* determine primes needed for factorisation */
    uint32_t limit = (uint32_t)(sqrt((long double)x) + 1.0L);
    extend_primes_to(limit);

    /* factorise x */
    for (size_t i = 0; i < prime_count && (uint64_t)prime_table[i] * prime_table[i] <= x; ++i) {
        uint32_t p = prime_table[i];
        if (x % p == 0) {
            /* distinct odd prime factor found */
            if (p != 2) {
                if (p % 8 == 1 || p % 8 == 7) {
                    result += result;          /* multiply by 2 */
                } else {
                    return (seq_t)0;            /* p ≡ 3 or 5 (mod 8) */
                }
            }
            /* remove all occurrences of p */
            while (x % p == 0)
                x /= p;
        }
    }

    /* remaining prime factor larger than sqrt(original n) */
    if (x > 1) {
        uint32_t p = (uint32_t)x;             /* x fits in uint32_t for n ≤ 10^9 */
        if (p != 2) {
            if (p % 8 == 1 || p % 8 == 7) {
                result += result;              /* multiply by 2 */
            } else {
                return (seq_t)0;
            }
        }
    }

    return result;
}
