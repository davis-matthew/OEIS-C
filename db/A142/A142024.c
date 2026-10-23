/*
 * A142024  Primes congruent to 20 mod 31.
 * https://oeis.org/A142024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime congruent to 20 modulo 31 by trial division */
/* a(1) = 113, a(2) = 547, ... */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t small_primes[500];
static size_t small_primes_count;

/* initialise a table of primes up to 2000 (sufficient for sqrt of
   the largest candidate needed for n ≤ 10000) */
static void init_small_primes(void)
{
    static bool done = false;
    if (done)
        return;
    const size_t MAX_PRIME = 2000;
    bool sieve[MAX_PRIME + 1];
    for (size_t i = 0; i <= MAX_PRIME; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (size_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= MAX_PRIME; j += i)
                sieve[j] = false;
        }
    }
    small_primes_count = 0;
    for (size_t i = 2; i <= MAX_PRIME; ++i) {
        if (sieve[i]) {
            small_primes[small_primes_count++] = (uint32_t)i;
        }
    }
    done = true;
}

/* test primality of n by trial division using the pre‑computed primes */
static bool is_prime_candidate(uint64_t n)
{
    if (n < 2)
        return false;
    for (size_t i = 0; i < small_primes_count; ++i) {
        uint64_t p = small_primes[i];
        if (p * p > n)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* external function required by the framework */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    init_small_primes();
    idx_t count = 0;
    uint64_t candidate = 20;          /* start from 20, next will be 51 */
    while (true) {
        candidate += 31;              /* numbers of form 31k+20 */
        if (is_prime_candidate(candidate)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
    }
}
