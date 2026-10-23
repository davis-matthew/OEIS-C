/*
 * A105453  Numbers k such that prime(k) + k*prime(k+1) is prime.
 * https://oeis.org/A105453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k (ordered by increasing k) such that prime(k) +
 *            k*prime(k+1) is prime. Offset is 1: a(1)=2, a(2)=14,
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   isPrime64, isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime64.h"
#include "../utils/isprime_big.h"

/* global arrays for primes and found terms */
static idx_t *prime_array   = NULL;   /* 1-based primes: prime_array[i-1] = prime(i) */
static idx_t prime_count    = 0;
static idx_t prime_capacity = 0;
static idx_t next_prime_candidate = 5; /* next odd to test for primality */

static seq_t *term_array   = NULL;    /* 0-based array of sequence values */
static idx_t  term_size    = 0;
static idx_t  term_capacity = 0;
static idx_t  last_k_tested = 1;      /* last k that was examined */

/* Ensure prime_array contains at least k primes (indices 1..k). */
static void ensure_primes(idx_t k)
{
    if (prime_count >= k)
        return;

    if (prime_capacity == 0) {
        prime_capacity = 4;
        prime_array = (idx_t*)malloc(prime_capacity * sizeof(idx_t));
        prime_array[0] = 2;
        prime_array[1] = 3;
        prime_count = 2;
    }

    while (prime_count < k) {
        /* grow storage if needed */
        if (prime_count == prime_capacity) {
            prime_capacity <<= 1;
            prime_array = (idx_t*)realloc(prime_array,
                                          prime_capacity * sizeof(idx_t));
        }
        /* find next prime */
        while (!isPrime64(next_prime_candidate))
            next_prime_candidate += 2;
        prime_array[prime_count++] = next_prime_candidate;
        next_prime_candidate += 2;
    }
}

/* Return the n-th term of the sequence (n is 1‑based). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* lazy initialization */
    if (term_array == NULL) {
        term_capacity = 64;
        term_array = (seq_t*)malloc(term_capacity * sizeof(seq_t));
    }
    if (prime_array == NULL)
        ensure_primes(2);  /* prime(1) and prime(2) are needed */

    /* advance until we have found n terms */
    while (term_size < n) {
        ++last_k_tested;          /* candidate k */
        if (last_k_tested == 1)   /* k = 1 is not part of the sequence */
            continue;

        /* we need primes up to index k+1 */
        ensure_primes(last_k_tested + 1);

        idx_t pk  = prime_array[last_k_tested - 1];     /* prime(k)   */
        idx_t pk1 = prime_array[last_k_tested];         /* prime(k+1) */

        /* Compute value = pk + k * pk1 safely in 128‑bit, then to seq_t */
        unsigned __int128 val128 = (unsigned __int128)pk +
                                   (unsigned __int128)last_k_tested *
                                   (unsigned __int128)pk1;
        seq_t val = (seq_t)val128;

        /* Test primality */
        bool prime_flag;
        if (val128 <= (unsigned __int128)UINT64_MAX)
            prime_flag = isPrime64((uint64_t)val128);
        else
            prime_flag = isprime_big(val);

        if (prime_flag) {
            if (term_size == term_capacity) {
                term_capacity <<= 1;
                term_array = (seq_t*)realloc(term_array,
                                             term_capacity * sizeof(seq_t));
            }
            term_array[term_size++] = (seq_t)last_k_tested;
        }
    }

    return term_array[n - 1];
}
