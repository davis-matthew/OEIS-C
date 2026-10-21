/*
 * A060417  Number of different decimal digits in n-th prime.
 * https://oeis.org/A060417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of distinct decimal digits in the n‑th prime
   (offset 1).  It builds the list of primes lazily using trial division. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t *primes = NULL;      /* dynamic array of primes          */
static size_t    prime_cnt = 0;      /* current number of primes stored  */
static size_t    prime_cap = 0;      /* capacity of the array            */

/* Ensure that primes array holds at least n primes (1‑based). */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;

    /* Grow the array if necessary. */
    if (prime_cap < (size_t)n) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes = tmp;
        prime_cap = new_cap;
    }

    /* Start generating from the next integer after the last prime. */
    uint64_t candidate = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;

    while (prime_cnt < (size_t)n) {
        bool is_prime = true;

        /* Test divisibility by known primes up to sqrt(candidate). */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime. */
            primes[prime_cnt++] = candidate;
        }

        ++candidate;   /* Move to the next candidate integer. */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* the sequence is defined for n ≥ 1 */

    ensure_primes(n);

    uint64_t p = primes[n - 1];
    uint16_t mask = 0;

    /* Build a bitmask of the digits appearing in p. */
    while (p > 0) {
        mask |= 1u << (uint8_t)(p % 10);
        p /= 10;
    }

    /* Count the set bits in mask. */
    unsigned count = 0;
    while (mask) {
        ++count;
        mask &= mask - 1;
    }

    return (seq_t)count;
}
