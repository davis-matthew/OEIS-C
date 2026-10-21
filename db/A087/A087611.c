/*
 * A087611  a(n) = (prime(n) - 1) mod n.
 * https://oeis.org/A087611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n) - 1) mod n (offset 1)
 * Range:     u8 n <= 344, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: dynamic list of primes */
static uint64_t *prime_list = NULL;   /* array of primes, 1‑based indexed in logic */
static size_t prime_cnt  = 0;         /* number of primes stored */
static size_t prime_cap  = 0;         /* capacity of prime_list */
static uint64_t next_candidate = 2;   /* next integer to test for primality */

/* Ensure that at least n primes are precomputed */
static void ensure_primes(idx_t n)
{
    if (n <= prime_cnt) return;          /* already have enough */

    /* Estimate an upper bound for the nth prime to preallocate */
    size_t extra = n - prime_cnt;
    size_t new_cap = prime_cap ? prime_cap : 128;
    while (new_cap < prime_cnt + extra) new_cap <<= 1;  /* double until enough */

    if (new_cap > prime_cap) {
        uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) exit(1);                 /* out of memory */
        prime_list = tmp;
        prime_cap = new_cap;
    }

    /* Generate primes until we have n of them */
    while (prime_cnt < n) {
        uint64_t candidate = next_candidate;
        int is_prime = 1;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = 0; break; }
        }

        if (is_prime) {
            prime_list[prime_cnt++] = candidate;
        }
        next_candidate = candidate + 1;
    }
}

/* Return the nth prime (n >= 1) */
static uint64_t nth_prime(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but return 0 for safety */
    ensure_primes(n);
    return prime_list[n - 1];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safe default */
    uint64_t p = nth_prime(n);
    return (seq_t)((p - 1) % n);
}
