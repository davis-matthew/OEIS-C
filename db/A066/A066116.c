/*
 * A066116  a(n) = prime(n-2)*prime(n-1)^2*prime(n).
 * https://oeis.org/A066116
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000040(n-2) * A000040(n-1)^2 * A000040(n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 55, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000040(k) is the k‑th prime (k ≥ 1).  n ≥ 3, a(3)=90. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static storage for primes */
static uint32_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;

/* Ensure that the list contains at least the first n primes. */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n) return;

    /* grow storage if needed */
    if (prime_capacity < (size_t)n) {
        size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) abort();           /* allocation failure */
        prime_list = tmp;
        prime_capacity = new_cap;
    }

    /* start checking from the next integer after the last known prime */
    uint32_t candidate = prime_count ? prime_list[prime_count - 1] + 1 : 2;
    while (prime_count < (size_t)n) {
        bool is_prime = true;
        /* test divisibility by known primes up to sqrt(candidate) */
        for (size_t i = 0; i < prime_count; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    /* Valid only for n >= 3; behaviour for smaller n is irrelevant */
    if (n < 3) return 0;

    /* Ensure we have enough primes */
    ensure_primes(n);

    /* indices: prime_list[0] = 2 (prime 1) */
    uint32_t p1 = prime_list[(size_t)(n - 3)]; /* A000040(n-2) */
    uint32_t p2 = prime_list[(size_t)(n - 2)]; /* A000040(n-1) */
    uint32_t p3 = prime_list[(size_t)(n - 1)]; /* A000040(n)   */

    seq_t result = (seq_t)p1;
    result *= (seq_t)p2;
    result *= (seq_t)p2;
    result *= (seq_t)p3;
    return result;
}
