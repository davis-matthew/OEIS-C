/*
 * A085623  Let p = n-th prime; a(n) = number of pairs (i,j) with 0 < i < p, 0
 *          < j < p such that i*j == 1 mod p and i and j have opposite parity.
 * https://oeis.org/A085623
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of ordered pairs (i,j), 0 < i < p, 0 < j < p, with
 *            i*j ≡ 1 (mod p) and i, j of opposite parity, where p is the n-th
 *            prime. (a(2) is the first term.)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/mod_pow.h"

/* ----------------------------------------------------------------------
   Prime storage and helper
   ---------------------------------------------------------------------- */
static idx_t *primes = NULL;     /* 1‑based array of primes       */
static size_t primes_cnt = 0;    /* number of primes stored       */
static size_t primes_cap = 0;    /* current capacity              */

/* Make sure we have at least n primes in the array.  Simple
   trial‑division generator. */
static void ensure_n_primes(idx_t n)
{
    if (primes_cnt >= n)
        return;

    if (primes_cnt >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }

    idx_t candidate = primes_cnt ? primes[primes_cnt - 1] + 1 : 2;
    while (primes_cnt < n) {
        if (candidate == 2) {
            primes[primes_cnt++] = 2;
            candidate = 3;
            continue;
        }
        if ((candidate & 1) == 0) {  /* skip evens */
            ++candidate;
            continue;
        }
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            idx_t q = primes[i];
            if (q * q > candidate)
                break;
            if (candidate % q == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_cnt >= primes_cap) {
                size_t new_cap = primes_cap * 2;
                idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                if (!tmp) abort();
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = candidate;
        }
        ++candidate;
    }
}

static idx_t nth_prime(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined, but never used */
    ensure_n_primes(n);
    return primes[n - 1];
}

/* ----------------------------------------------------------------------
   Sequence function
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    idx_t p = nth_prime(n);
    seq_t count = 0;

    for (idx_t i = 1; i < p; ++i) {
        seq_t inv = mod_pow((seq_t)i, p - 2, (seq_t)p);
        uint64_t inv_parity = (uint64_t)(inv & 1);
        uint64_t i_parity   = (uint64_t)(i & 1);
        if (inv_parity != i_parity)
            ++count;
    }
    return count;
}
