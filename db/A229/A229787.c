/*
 * A229787  a(n) = n-th prime modulo 24.
 * https://oeis.org/A229787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n‑th prime modulo 24
 * Range:     u8 n <= 1900, u16 n <= 1900, u32 n <= 1900, u64 n <= 1900,
 *            u128 n <= 1900, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function relies on the provided helper:
 *   bool is_prime(idx_t x);
 * which declares primality testing for an unsigned integer of type idx_t. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime.h"

/* ------------------- prime cache ------------------- */
static uint64_t *prime_list = NULL;   /* array of primes               */
static size_t   prime_cap  = 0;       /* allocated capacity            */
static idx_t    prime_cnt  = 0;       /* number of primes already stored */
static uint64_t last_checked = 1;     /* last integer examined */

/* Grow the cache when more space is needed. */
static void grow_prime_cache(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);                 /* allocation failed */
    prime_list = tmp;
    prime_cap = new_cap;
}

/* Ensure that at least n primes have been generated. */
static void ensure_n_primes(idx_t n)
{
    if (prime_cnt >= n) return;                  /* already have enough */

    while (prime_cnt < n) {
        /* Advance to the next candidate prime number. */
        if (last_checked < 2) {
            last_checked = 2;
        } else if (last_checked == 2) {
            last_checked = 3;
        } else {
            last_checked += 2;                  /* skip even numbers after 2 */
        }

        if (is_prime((idx_t)last_checked)) {
            if (prime_cnt == prime_cap) grow_prime_cache();
            prime_list[prime_cnt++] = last_checked;
        }
    }
}

/* ------------------- sequence function ------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                   /* undefined, but harmless */
    ensure_n_primes(n);
    uint64_t mod = prime_list[n - 1] % 24U;        /* n‑th prime modulo 24 */
    return (seq_t)mod;
}
