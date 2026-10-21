/*
 * A063442  Primes of the form p*q+4 where p and q are consecutive primes.
 * https://oeis.org/A063442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 370, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_seq, isPrime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063442: primes of the form p*q+4 where p and q are consecutive primes */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */
#include "../utils/isPrime.h"      /* bool isPrime(idx_t n); */
#include "../utils/append_seq.h"   /* void append_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val); */

seq_t a(idx_t n)
{
    /* Edge case: offset 1, return 0 for n == 0 (outside defined range) */
    if (n == 0) return (seq_t)0;

    /* static cache of computed terms */
    static seq_t *term_cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* static state of the prime pair generation */
    static uint64_t prev_prime = 0;      /* last prime used as 'p' in pair (p, q) */
    static uint64_t curr_prime = 0;      /* last prime used as 'q' in pair (p, q) */

    /* Generate terms until we have at least n terms in the cache */
    while (cache_len < n)
    {
        /* Initialization of the first pair (2,3) */
        if (prev_prime == 0)
        {
            prev_prime = 2;
            curr_prime = next_prime(prev_prime);   /* should be 3 */
        }

        /* Compute p*q + 4 */
        uint64_t prod = prev_prime * curr_prime + 4;

        /* Test primality (product fits in 64 bits for n <= 10000, otherwise cast to idx_t) */
        if (isPrime((idx_t)prod))
            append_seq(&term_cache, &cache_len, &cache_cap, (seq_t)prod);

        /* Advance to the next consecutive prime pair */
        uint64_t nextp = next_prime(curr_prime);
        prev_prime = curr_prime;
        curr_prime = nextp;
    }

    return term_cache[n - 1];
}
