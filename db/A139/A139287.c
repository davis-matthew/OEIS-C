/*
 * A139287  2^(2p - 1) - 1, where p is prime.
 * https://oeis.org/A139287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*p_n - 1) - 1 (where p_n is the n-th prime)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"   /* for big_pow(seq_t, seq_t) */

static uint64_t *prime_list = NULL;   /* dynamic array of primes */
static size_t prime_cnt   = 0;        /* number of primes stored */
static size_t prime_cap   = 0;        /* capacity of prime_list */

/* Ensure that prime_list contains at least n primes (p_1 … p_n). */
static void ensure_primes(idx_t n)
{
    while (prime_cnt < (size_t)n) {
        /* Find next prime after the last one (or start at 2). */
        uint64_t candidate = (prime_cnt == 0) ? 2 : prime_list[prime_cnt - 1] + 1;

        /* Test primality using the primes already found. */
        while (true) {
            bool is_prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = prime_list[i];
                if (p * p > candidate)
                    break;
                if (candidate % p == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                break;          /* candidate is prime */
            ++candidate;
        }

        /* Store the new prime, expanding the array if necessary. */
        if (prime_cnt == prime_cap) {
            size_t new_cap = (prime_cap == 0) ? 64 : prime_cap * 2;
            uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
            if (!tmp) abort();         /* out‑of‑memory, terminate */
            prime_list = tmp;
            prime_cap = new_cap;
        }
        prime_list[prime_cnt++] = candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined by OEIS, but harmless */

    /* Make sure we have the n-th prime. */
    ensure_primes(n);

    uint64_t p = prime_list[n - 1];          /* the n-th prime */
    seq_t exp = (seq_t)2 * (seq_t)p - (seq_t)1;   /* exponent = 2*p - 1 */
    seq_t val = big_pow((seq_t)2, exp);          /* 2^(2p-1) */
    val -= (seq_t)1;                            /* minus 1 */
    return val;
}
