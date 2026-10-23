/*
 * A141653  Composites of the form n/(p(n)-p(n-1)), where p(n)=n-th prime.
 * https://oeis.org/A141653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth composite value of n/(p(n)-p(n-1)),
   where p(n) is the nth prime. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"      /* is_prime(idx_t)   */
#include "../utils/is_prime_big.h"  /* is_prime_big(seq_t) */

/* static storage for generated primes (p(1) = 2) */
static uint64_t *primes     = NULL;
static size_t   primes_len  = 0;
static size_t   primes_cap  = 0;

/* static storage for computed sequence terms */
static seq_t *terms      = NULL;
static size_t terms_len  = 0;
static size_t terms_cap  = 0;

/* next prime index to be processed (k = 2,3,4,…) */
static idx_t next_k = 2;

/* append a prime to the primes array, resizing as necessary */
static void append_prime(uint64_t p)
{
    if (primes_len == primes_cap) {
        size_t newcap = primes_cap ? primes_cap * 2 : 16;
        primes = (uint64_t*)realloc(primes, newcap * sizeof(uint64_t));
        primes_cap = newcap;
    }
    primes[primes_len++] = p;
}

/* generate the next prime and store it */
static void generate_next_prime(void)
{
    uint64_t candidate = (primes_len == 0) ? 2ULL : primes[primes_len - 1] + 1ULL;
    while (!is_prime(candidate))
        ++candidate;
    append_prime(candidate);
}

/* ensure that at least `need` composite terms have been computed */
static void ensure_terms(size_t need)
{
    while (terms_len < need) {
        /* make sure primes up to p(next_k) are available */
        while (primes_len < (size_t)next_k)
            generate_next_prime();

        /* compute p(next_k) – p(next_k-1) */
        uint64_t diff = primes[(size_t)next_k - 1] - primes[(size_t)next_k - 2];

        /* n divisible by the denominator? */
        if (diff != 0 && (next_k % diff) == 0) {
            seq_t val = ((seq_t)next_k) / ((seq_t)diff);
            /* accept only composite numbers > 1 */
            if (val > (seq_t)1 && !is_prime_big(val)) {
                if (terms_len + 1 > terms_cap) {
                    size_t newcap = terms_cap ? terms_cap * 2 : 16;
                    terms = (seq_t*)realloc(terms, newcap * sizeof(seq_t));
                    terms_cap = newcap;
                }
                terms[terms_len++] = val;
            }
        }
        ++next_k;
    }
}

/* PUBLIC SEQUENCE FUNCTION */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out of range; undefined */
    ensure_terms((size_t)n);
    return terms[(size_t)n - 1];
}
