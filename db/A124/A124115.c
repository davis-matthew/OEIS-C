/*
 * A124115  a(n) = 2*prime(n) - prime(n+1) + 2.
 * https://oeis.org/A124115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*prime(n) - prime(n+1) + 2 * Offset: 1 * Compute using a
 *            lazy prime sieve that expands on demand
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Static state for the simple prime generator */
static uint64_t *prime_vals = NULL;
static size_t prime_count = 0;
static size_t prime_capacity = 0;

/* Append a new prime to the cache, growing its storage if necessary */
static void append_prime(uint64_t p)
{
    if (prime_count == prime_capacity) {
        size_t new_cap = prime_capacity ? prime_capacity * 2 : 16;
        uint64_t *new_arr = (uint64_t*)realloc(prime_vals, new_cap * sizeof(uint64_t));
        if (!new_arr) {
            /* In a contest judge, abort silently on allocation failure */
            return;
        }
        prime_vals = new_arr;
        prime_capacity = new_cap;
    }
    prime_vals[prime_count++] = p;
}

/* Generate primes up to at least the given count using trial division */
static void ensure_primes(size_t needed)
{
    if (prime_count >= needed) return;

    uint64_t candidate = prime_count ? prime_vals[prime_count - 1] + 1 : 2;
    if (prime_count == 0) {
        /* The first prime is 2 */
        append_prime(2);
        candidate = 3;
    }

    while (prime_count < needed) {
        int is_prime = 1;
        /* trial division by known primes up to sqrt(candidate) */
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_vals[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            append_prime(candidate);
        }
        ++candidate;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* Undefined for n = 0; return 0 */
    size_t needed = (size_t)(n + 1);   /* Need prime(n) and prime(n+1) */
    ensure_primes(needed);

    /* primes are stored 0‑indexed: prime(1) is prime_vals[0] */
    seq_t pn  = (seq_t)prime_vals[(size_t)(n - 1)];
    seq_t pn1 = (seq_t)prime_vals[(size_t)(n)];
    return (seq_t)2 * pn - pn1 + (seq_t)2;
}
