/*
 * A094517  Primes p such that 6p+11 is not a prime.
 * https://oeis.org/A094517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 731, u32 n <= 975, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_uint64_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 6p+11 is NOT a prime.
   a(1) = 11, a(2) = 19, a(3) = 29, ...   (OEIS A094517, offset 1) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
/* util for primality test on arbitrary-size integers */
#include "../utils/is_prime_big.h"
#include "../utils/ensure_uint64_capacity.h"

static void ensure_seq_capacity(size_t needed, size_t *cap, seq_t **buf)
{
    if (needed <= *cap) return;
    size_t newcap = (*cap == 0) ? 16 : *cap << 1;
    while (newcap < needed) newcap <<= 1;
    *buf = (seq_t *)realloc(*buf, newcap * sizeof(seq_t));
    *cap = newcap;
}

/*--- static state -----------------------------------------------------*/
static uint64_t cur = 2;                /* next integer to test for primality */
static uint64_t *prime_cache = NULL;    /* list of primes found so far */
static size_t prime_len = 0;            /* how many primes in prime_cache   */
static size_t prime_cap = 0;            /* capacity of prime_cache          */

static seq_t *desired = NULL;           /* list of special primes           */
static size_t desired_len = 0;          /* how many are stored              */
static size_t desired_cap = 0;          /* capacity of desired array        */

/*--- helper: test primality of 64‑bit integer using cached primes -------*/
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i)
    {
        uint64_t p = prime_cache[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/*--- main sequence function -------------------------------------------*/
seq_t a(idx_t n)
{
    /* iterate until we have at least n special primes */
    while (desired_len < n)
    {
        /* test if cur is prime */
        if (is_prime_uint64(cur))
        {
            /* store it in the prime cache */
            ensure_uint64_capacity(prime_len + 1, &prime_cap, &prime_cache);
            prime_cache[prime_len++] = cur;

            /* compute 6*cur + 11 as seq_t and test composite */
            seq_t val = (seq_t)cur * (seq_t)6 + (seq_t)11;
            if (!is_prime_big(val))
            {
                /* store the special prime */
                ensure_seq_capacity(desired_len + 1, &desired_cap, &desired);
                desired[desired_len++] = (seq_t)cur;
            }
        }

        ++cur; /* move to next candidate */
    }

    /* return the (n‑1)‑indexed element (a(1) is first) */
    return desired[n - 1];
}
