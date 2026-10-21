/*
 * A230849  A075526 and A000012 interleaved.
 * https://oeis.org/A230849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2k) = 1; a(2k-1) = p_k - p_(k-1) (p_0 = 1, p_k = k‑th prime)
 * Range:     u8 n <= 8844, u16 n <= 8844, u32 n <= 8844, u64 n <= 8844,
 *            u128 n <= 8844, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global prime cache */
static uint64_t *prime_arr = NULL;   /* array of primes, prime_arr[0] = 2, ...            */
static size_t     prime_cnt = 0;     /* number of primes currently stored               */
static size_t     prime_cap = 0;     /* current allocation size (in primes)             */

/* Ensure that prime_arr contains at least primes[0 … upto_index] (i.e. prime_cnt > upto_index). */
static void prime_ensure_capacity(size_t upto_index)
{
    if (prime_arr == NULL) {
        /* initial allocation: 16 primes should cover many test cases */
        prime_cap = 16;
        prime_arr = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_arr) abort();
        prime_arr[0] = 2;          /* first prime */
        prime_cnt = 1;
    }

    /* Generate additional primes until prime_cnt > upto_index */
    while (prime_cnt <= upto_index) {
        /* grow storage if necessary */
        if (prime_cnt >= prime_cap) {
            prime_cap <<= 1;
            uint64_t *tmp = (uint64_t*)realloc(prime_arr, prime_cap * sizeof(uint64_t));
            if (!tmp) abort();
            prime_arr = tmp;
        }

        /* test candidates starting just above the last prime */
        uint64_t cand = prime_arr[prime_cnt - 1] + 1;
        while (true) {
            if (cand % 2 == 0) { ++cand; continue; } /* skip even numbers */
            bool prime = true;
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = prime_arr[i];
                if (p > cand / p) break;  /* p*p > cand */
                if (cand % p == 0) { prime = false; break; }
            }
            if (prime) break; /* found next prime */
            ++cand;
        }
        prime_arr[prime_cnt++] = cand;
    }
}

/* Return the n-th term of A230849.  a(1) is the first term.            */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* 0 is outside the offset – never used in tests */

    /* Even index → vertical edge of the staircase.  All are 1. */
    if ((n & 1ULL) == 0) return (seq_t)1;

    /* odd index n = 2k-1 → horizontal edge.  Compute p_k - p_(k-1). */
    size_t k = (size_t)((n - 1) >> 1);  /* k = (n-1)/2, zero‑based: k=0 for n=1 */
    if (k == 0) return (seq_t)1;       /* a(1) = 1 = 2 - 1  (p_0 = 1) */

    /* Ensure we have p_k and p_(k-1) in the cache. */
    prime_ensure_capacity(k);

    uint64_t diff = prime_arr[k] - prime_arr[k - 1]; /* difference of consecutive primes */
    return (seq_t)diff;
}
