/*
 * A027883  Positions of primes in sequence (A246655) of primes and prime
 *          powers {p^i, i >= 1}.
 * https://oeis.org/A027883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime powers <= p_n where p_n is the n-th prime
 * Range:     u8 n <= 227, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Persistent prime cache */
static uint64_t *primes = NULL;      /* array of discovered primes   */
static size_t primes_cap = 0;        /* current capacity of array    */
static size_t primes_count = 0;      /* number of primes stored      */
static uint64_t next_candidate = 2;  /* next integer to test for prime */

/* Grow the primes array when needed                           */
static void _grow_primes(void)
{
    size_t new_cap = primes_cap ? primes_cap * 2 : 64;
    uint64_t *tmp = (uint64_t*)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);   /* failed allocation */
    primes = tmp;
    primes_cap = new_cap;
}

/* Find the n‑th prime (1‑based).  The primes array persists across calls. */
static uint64_t _nth_prime(idx_t n)
{
    while (primes_count < (size_t)n) {
        uint64_t candidate = next_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_count == primes_cap) _grow_primes();
            primes[primes_count++] = candidate;
        }
        ++next_candidate;
    }
    return primes[(size_t)n - 1];
}

/* Count all prime powers <= limit using the cached primes. */
static seq_t _prime_power_count(uint64_t limit)
{
    seq_t count = 0;
    for (size_t i = 0; i < primes_count && primes[i] <= limit; ++i) {
        uint64_t p = primes[i];
        unsigned __int128 val = p;         /* start with p^1 */
        while (val <= limit) {
            ++count;                       /* count this power   */
            val *= p;                      /* next power */
        }
    }
    return count;
}

/* Public entry point */
seq_t a(idx_t n)
{
    uint64_t p = _nth_prime(n);
    return _prime_power_count(p);
}
