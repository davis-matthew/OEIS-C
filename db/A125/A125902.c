/*
 * A125902  a(n) = sum of the first n primes which are coprime to n.
 * https://oeis.org/A125902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the first n primes which are coprime to n
 * Range:     u8 n <= 11, u16 n <= 155, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* Prime storage */
static uint32_t *primes = NULL;          /* array of primes seen so far   */
static size_t prime_count  = 0;          /* number of primes in 'primes'  */
static size_t prime_capacity = 0;        /* allocated size of 'primes'    */
static uint32_t next_candidate = 0;      /* next number to test for primality */

/* Ensure the prime array can hold at least 'needed' entries */
static void ensure_prime_capacity(size_t needed)
{
    if (needed <= prime_capacity)
        return;
    size_t new_cap = prime_capacity;
    if (new_cap < 16)
        new_cap = 16;
    while (new_cap < needed)
        new_cap <<= 1;          /* grow by powers of two */
    uint32_t *new_arr = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
    if (!new_arr)
        exit(1);               /* fatal error: out of memory */
    primes = new_arr;
    prime_capacity = new_cap;
}

/* Test whether a candidate is prime using the primes we already know */
static bool is_candidate_prime(uint32_t candidate)
{
    if (candidate < 2)
        return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        /* use 64‑bit multiplication to avoid overflow */
        if ((uint64_t)p * (uint64_t)p > (uint64_t)candidate)
            break;
        if (candidate % p == 0)
            return false;
    }
    return true;
}

/* Generate the next prime and store it into the prime array */
static void generate_next_prime(void)
{
    while (1) {
        if (next_candidate <= 2) {
            next_candidate = 3;
            ensure_prime_capacity(prime_count + 1);
            primes[prime_count++] = 2;
            return;
        }
        uint32_t cand = next_candidate;
        next_candidate += 2;     /* only test odd numbers */
        if (is_candidate_prime(cand)) {
            ensure_prime_capacity(prime_count + 1);
            primes[prime_count++] = cand;
            return;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset starts at 1; undefined for n=0 */
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    idx_t chosen = 0;    /* how many coprime primes we have summed so far */
    size_t idx = 0;      /* index into primes array */

    while (chosen < n) {
        /* Extend the primes array if needed */
        if (idx == prime_count)
            generate_next_prime();

        uint32_t p = primes[idx++];
        /* Check coprimality with n */
        if (gcd((seq_t)p, (seq_t)n) == (seq_t)1) {
            sum += (seq_t)p;
            ++chosen;
        }
    }
    return sum;
}
