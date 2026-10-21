/*
 * A212361  Number of primes between two consecutive prime triples (p, p+2,
 *          p+6).
 * https://oeis.org/A212361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes strictly between successive prime
 *            triples (p, p+2, p+6). The function generates primes up to a
 *            dynamic bound, collects all prime triples (p is the starting
 *            prime of the triple), and for a given n (1‑based) returns the
 *            count of primes between triple n and triple n+1
 * Range:     u8 n <= 365, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/is_prime_bit.h"

/* static state used for memoisation */
static size_t sieve_limit          = 0;
static uint8_t  *prime_bits        = NULL;
static uint32_t *triples           = NULL;
static size_t   triple_capacity    = 0;
static size_t   triple_count       = 0;
static idx_t    max_n_needed       = 0;

/* build prime bitset and list of prime triples up to `limit`.
   Reallocates necessary arrays. */
static void build_sieve_and_triples(size_t limit)
{
    /* free old structures */
    free(prime_bits);
    free(triples);

    /* create bitset, all bits set initially */
    size_t byte_len = (limit >> 3) + 1;
    prime_bits = (uint8_t *)malloc(byte_len);
    memset(prime_bits, 0xFF, byte_len);
    /* clear 0 and 1 */
    prime_bits[0] &= ~3;

    /* sieve */
    size_t sqrtL = (size_t)sqrt((double)limit) + 1;
    for (size_t i = 2; i <= sqrtL; ++i) {
        if (is_prime_bit(prime_bits, i)) {
            for (size_t j = i * i; j <= limit; j += i)
                prime_bits[j >> 3] &= (uint8_t)~(1 << (j & 7));
        }
    }

    /* collect triples */
    triple_capacity = limit / 10 + 10;  /* initial guess */
    triples = (uint32_t *)malloc(triple_capacity * sizeof(uint32_t));
    triple_count = 0;
    for (size_t i = 2; i + 6 <= limit; ++i) {
        if (is_prime_bit(prime_bits, i) &&
            is_prime_bit(prime_bits, i + 2) &&
            is_prime_bit(prime_bits, i + 6)) {
            if (triple_count == triple_capacity) {
                triple_capacity <<= 1;
                triples = (uint32_t *)realloc(triples,
                                              triple_capacity * sizeof(uint32_t));
            }
            triples[triple_count++] = (uint32_t)i;
        }
    }
}

/* Ensure the internal tables contain at least the first (n+1) prime triples */
static void ensure_triples(idx_t n)
{
    if (n <= max_n_needed) return;            /* already have enough */
    max_n_needed = n;                         /* store largest n seen */

    /* if we already have enough, just return */
    if (triple_count > (size_t)n) return;

    /* otherwise, grow sieve limit until we get enough triples */
    size_t new_limit = sieve_limit ? sieve_limit * 2 : 200;
    while (1) {
        sieve_limit = new_limit;
        build_sieve_and_triples(sieve_limit);
        if (triple_count > (size_t)n) break;   /* enough triples obtained */
        new_limit <<= 1;                       /* double limit and retry */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* outside OEIS offset */

    /* we need triples[0 .. n]  (zero‑based) */
    ensure_triples(n);                     /* make sure we have enough data */

    /* get the two successive triples */
    uint32_t p = triples[n - 1];
    uint32_t q = triples[n];

    /* count primes strictly between p+6 and q */
    idx_t count = 0;
    for (idx_t x = p + 7; x < q; ++x)
        if (is_prime_bit(prime_bits, x))
            ++count;

    return (seq_t)count;
}
