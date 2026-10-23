/*
 * A106073  Primes p such that 3*p+20 and 20*p+3 are primes.
 * https://oeis.org/A106073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 3*p+20 and 20*p+3 are primes */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static seq_t *primes = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint64_t next_candidate = 3;         /* start from the smallest odd prime candidate */

/* Ensure the primes array can hold at least 'needed' elements */
static inline void ensure_capacity(size_t needed)
{
    if (needed <= prime_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
    primes = new_ptr;
    prime_cap = new_cap;
}

/* Test whether p satisfies the defining condition */
static inline bool good_prime(uint64_t p)
{
    return is_prime(p) &&
           is_prime(3 * p + 20) &&
           is_prime(20 * p + 3);
}

seq_t a(idx_t n)
{
    /* Fill the table up to the requested index (non‑increasing calls are allowed) */
    while (prime_cnt < n)
    {
        if (good_prime(next_candidate))
        {
            ensure_capacity(prime_cnt + 1);
            primes[prime_cnt++] = (seq_t)next_candidate;
        }
        next_candidate += 2;  /* skip even numbers */
    }
    return primes[n - 1];
}
