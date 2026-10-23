/*
 * A171018  In the sequence of prime numbers, replace all the '3' digits with
 *          '2' and vice versa.
 * https://oeis.org/A171018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   swap23
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171018: replace every digit 2 with 3 and every digit 3 with 2
   in the n‑th prime number (OEIS offset 1). */

#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>  /* bool */
#include "../utils/swap23.h"

static idx_t *prime_list = NULL;   /* list of discovered primes */
static size_t prime_list_cnt = 0;  /* number of primes stored */
static size_t prime_list_cap = 0;  /* allocated capacity */

/* Append a prime to the static list, reallocating if needed. */
static void add_prime(idx_t p)
{
    if (prime_list_cnt >= prime_list_cap) {
        size_t new_cap = prime_list_cap ? prime_list_cap * 2 : 16;
        prime_list = (idx_t*)realloc(prime_list, new_cap * sizeof(idx_t));
        prime_list_cap = new_cap;
    }
    prime_list[prime_list_cnt++] = p;
}

/* Ensure that at least n primes have been generated using trial division. */
static void ensure_n_primes(idx_t n)
{
    if (prime_list_cnt >= (size_t)n) return;

    if (prime_list_cnt == 0) {
        add_prime(2);
        if (prime_list_cnt >= (size_t)n) return;
        add_prime(3);
        if (prime_list_cnt >= (size_t)n) return;
    }

    idx_t candidate = prime_list[prime_list_cnt - 1] + 1;
    for (; prime_list_cnt < (size_t)n; ++candidate) {
        if (candidate % 2 == 0) continue;          /* skip even numbers */
        bool is_prime = true;
        /* trial division by stored primes up to sqrt(candidate) */
        for (size_t i = 0; i < prime_list_cnt; ++i) {
            idx_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) add_prime(candidate);
    }
}

/* Public sequence entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_n_primes(n);
    idx_t prime = prime_list[n - 1];
    return swap23((seq_t)prime);
}
