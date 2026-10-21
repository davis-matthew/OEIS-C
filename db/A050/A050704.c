/*
 * A050704  Composite numbers k with the property that k minus the sum of the
 *          prime factors of k is prime.
 * https://oeis.org/A050704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 7206, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A050704: Composite numbers k such that k - sum of the prime
   factors of k (with multiplicity) is prime.
   This code builds a linear sieve for smallest prime factors,
   then generates terms on demand. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrime.h"

static uint32_t *spf = NULL;          /* smallest prime factor for each i */
static size_t   spf_size = 0;         /* size of spf array (indices 0 … spf_size-1) */

/* Linear sieve for smallest prime factor up to N (inclusive).  The
   current global array is replaced with a new one. */
static void recompute_spf(size_t N)
{
    uint32_t *new_spf = (uint32_t*)calloc(N + 1, sizeof(uint32_t));
    if (!new_spf) return;              /* allocation failed – leave spf unchanged */

    /* Rough estimate of number of primes */
    size_t prime_cap = N / 10 + 1;
    uint32_t *primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!primes) {
        free(new_spf);
        return;
    }
    size_t prime_cnt = 0;

    for (size_t i = 2; i <= N; ++i) {
        if (new_spf[i] == 0) {
            new_spf[i] = (uint32_t)i;
            if (prime_cnt == prime_cap) {
                prime_cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, prime_cap * sizeof(uint32_t));
                if (!tmp) {                    /* allocation failed – clean up */
                    free(primes);
                    free(new_spf);
                    return;
                }
                primes = tmp;
            }
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (p > new_spf[i] || prod > N) break;
            new_spf[prod] = p;
        }
    }

    free(spf);
    free(primes);
    spf = new_spf;
    spf_size = N + 1;
}

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *terms = NULL;    /* stored terms */
    static size_t terms_cap = 0;   /* capacity of the array */
    static size_t terms_len = 0;   /* number of terms stored */
    static uint32_t current_k = 2; /* next integer to test */

    /* The sequence is defined for n >= 1. */
    if (n == 0) return (seq_t)0;

    /* Ensure spf table is large enough for current_k */
    if (current_k >= spf_size) {
        size_t new_limit = spf_size ? spf_size * 2 : 1024;
        while (new_limit <= current_k) new_limit <<= 1;
        recompute_spf(new_limit);
    }

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        /* Sum of prime factors of current_k (with multiplicity) */
        uint32_t tmp = current_k;
        seq_t sum_pf = 0;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            sum_pf += (seq_t)p;
            tmp /= p;
        }

        /* Check compositeness and the required property */
        if (spf[current_k] != current_k) { /* composite */
            seq_t diff_seq = (seq_t)current_k - sum_pf;
            idx_t diff = (idx_t)diff_seq;
            if (diff > 1 && isPrime(diff)) {
                /* store term */
                if (terms_len == terms_cap) {
                    terms_cap = terms_cap ? terms_cap << 1 : 16;
                    seq_t *tmp = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0; /* allocation failed */
                    terms = tmp;
                }
                terms[terms_len++] = (seq_t)current_k;
            }
        }

        ++current_k;
        if (current_k >= spf_size) {
            size_t new_limit = spf_size ? spf_size * 2 : 1024;
            while (new_limit <= current_k) new_limit <<= 1;
            recompute_spf(new_limit);
        }
    }

    return terms[n - 1];
}
