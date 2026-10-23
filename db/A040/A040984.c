/*
 * A040984  Primes p such that x^23 = 2 has no solution mod p.
 * https://oeis.org/A040984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 278, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: primes p ≡ 1 (mod 23) such that 2^((p‑1)/23) ≠ 1 (mod p).
   Each time a(n) is requested, we sieve primes up to a growing limit
   until the n‑th such prime has been found.  This is efficient enough
   for n ≤ 10000 (the primes involved are below a few million). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static uint64_t *prime_list = NULL;   /* dynamic array of primes */
static size_t prime_cap   = 0;        /* capacity of prime_list   */
static size_t prime_cnt   = 0;        /* number of primes stored  */
static idx_t   sieve_limit = 1;      /* highest integer already sieved   */

static void add_prime(uint64_t p)
{
    if (prime_cnt == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 64;
        uint64_t *tmp = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
        if (!tmp) { /* if realloc fails, abort (not expected) */
            exit(EXIT_FAILURE);
        }
        prime_list = tmp;
    }
    prime_list[prime_cnt++] = p;
}

/* Incrementally sieve integers from (sieve_limit+1) to limit inclusive.
   Only odd candidates are tested (except 2).  Prime divisors are
   taken from the primes we already have. */
static void sieve_up_to(idx_t limit)
{
    if (limit <= sieve_limit) return;

    idx_t start = sieve_limit + 1;
    if (start < 2) start = 2;

    for (idx_t cand = start; cand <= limit; ++cand) {
        if (cand == 2) {
            add_prime(2);
            continue;
        }
        if ((cand & 1) == 0) continue;   /* skip even numbers > 2 */

        bool is_prime = true;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint64_t p = prime_list[j];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) add_prime(cand);
    }
    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    /* Ensure we have at least some primes to start with. */
    static idx_t cur_limit = 1000;
    if (prime_cnt == 0) sieve_up_to(cur_limit);

    while (1) {
        size_t count = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            /* Only primes congruent to 1 modulo 23 are candidates. */
            if ((p - 1) % 23 == 0) {
                uint64_t k = (p - 1) / 23;
                /* 2^(k) mod p should not be 1 for the sequence prime. */
                if (mod_pow((seq_t)2, k, (seq_t)p) != (seq_t)1) {
                    ++count;
                    if (count == n) {
                        return (seq_t)p;
                    }
                }
            }
        }
        /* Not enough primes yet – double the sieve limit. */
        cur_limit *= 2;
        sieve_up_to(cur_limit);
    }
}
