/*
 * A142812  Primes congruent to 14 mod 61.
 * https://oeis.org/A142812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 105, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime congruent to 14 modulo 61. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* ---- static state ---- */
static seq_t *prime_list  = NULL;    /* array of primes ≡ 14 (mod 61)           */
static size_t prime_count = 0;       /* number of primes stored                */
static size_t prime_cap   = 0;       /* capacity of prime_list                 */
static idx_t   last_k      = 0;      /* next k so that candidate = 61*k+14     */

/* ---- ensure at least n primes are available ---- */
static void ensure_n_primes(idx_t n)
{
    /* Nothing if we already have enough. */
    if (prime_count >= (size_t)n)
        return;

    /* Lazy allocation of the list on first use. */
    if (prime_list == NULL) {
        prime_cap   = 128;
        prime_list  = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        if (!prime_list)
            abort();
    }

    /* Generate candidates until we reach the desired count. */
    while (prime_count < (size_t)n) {
        idx_t candidate = 61 * last_k + 14;   /* 61*k + 14   */

        /* No need to test 0‑based candidates less than 14. */
        if (candidate >= 14 && is_prime(candidate)) {
            if (prime_count >= prime_cap) {
                prime_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(prime_list, prime_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                prime_list = tmp;
            }
            prime_list[prime_count++] = (seq_t)candidate;
        }
        ++last_k;
    }
}

/* ---- public sequence function ---- */
seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; undefined for 0 */
        return (seq_t)0;

    ensure_n_primes(n);
    return prime_list[n - 1];
}
