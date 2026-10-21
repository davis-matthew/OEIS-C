/*
 * A098773  p*2^p + 1 where p is prime.
 * https://oeis.org/A098773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_n*2^p_n + 1, where p_n is the n‑th prime
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Global storage for the prime list. */
static idx_t *prime_list   = NULL; /* array of primes */
static size_t prime_cnt    = 0;     /* number of primes currently stored */
static size_t prime_cap    = 0;     /* current allocation size */
static idx_t  next_candidate = 2;   /* next integer to test for primality */

/* Test whether 'candidate' is prime using the primes already generated. */
static int is_prime_candidate(idx_t candidate)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p * p > candidate) break;
        if (candidate % p == 0) return 0;
    }
    return 1;
}

/* Ensure that at least 'n' primes have been generated and stored. */
static void ensure_primes(size_t n)
{
    if (prime_cnt >= n) return;

    if (!prime_list) {
        prime_cap = 32;
        prime_list = (idx_t *)malloc(sizeof(idx_t) * prime_cap);
    }

    while (prime_cnt < n) {
        if (is_prime_candidate(next_candidate)) {
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (idx_t *)realloc(prime_list, sizeof(idx_t) * prime_cap);
            }
            prime_list[prime_cnt++] = next_candidate;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n < 1, but safe */

    ensure_primes((size_t)n);

    idx_t p = prime_list[(size_t)(n - 1)];

    /* Compute 2^p */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < p; ++i) {
        pow2 = (seq_t)((pow2 * (seq_t)2));
    }

    return (seq_t)p * pow2 + (seq_t)1;
}
