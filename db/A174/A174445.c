/*
 * A174445  Numbers of the form (53^p-1)/52.
 * https://oeis.org/A174445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (53^p_n - 1) / 52 where p_n is the n‑th prime
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS sequence A174445.
   Utilises a simple prime sieve kept in static storage and the
   big_pow routine from the utils library. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/big_pow.h"

/* Static table of primes, expanded on demand. */
static uint64_t *prime_table = NULL;   /* array of primes            */
static size_t   prime_len   = 0;       /* number of primes stored    */
static size_t   prime_cap   = 0;       /* currently allocated cap    */

/* Expand prime_table so that it contains at least `need` primes. */
static void
ensure_primes(idx_t need)
{
    if (prime_len >= (size_t)need) return;

    /* Grow the allocation if necessary. */
    if (prime_cap < (size_t)need) {
        size_t new_cap = prime_cap ? prime_cap : 8;
        while (new_cap < (size_t)need) new_cap <<= 1;
        /* Cast required for C++ compilation. */
        uint64_t *tmp = (uint64_t *)realloc(prime_table,
                                            new_cap * sizeof(uint64_t));
        if (!tmp) abort();            /* allocation failure */
        prime_table = tmp;
        prime_cap   = new_cap;
    }

    /* Generate additional primes starting from the last known value. */
    uint64_t candidate = (prime_len == 0) ? 2U
                  : prime_table[prime_len - 1] + 1U;

    while (prime_len < (size_t)need) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_table[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_table[prime_len++] = candidate;
        }
        ++candidate;
    }
}

/* OEIS A174445: a(n) = (53^p_n - 1)/52 where p_n is the n-th prime. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    ensure_primes(n);                    /* guarantee p_n exists */

    /* `p` is the n-th prime. */
    seq_t p   = (seq_t)prime_table[(size_t)n - 1];
    seq_t pow = big_pow((seq_t)53, p);

    /* Compute (pow - 1) / 52 exactly. */
    seq_t res = (pow - (seq_t)1) / (seq_t)52;
    return res;
}
