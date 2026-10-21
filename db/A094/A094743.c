/*
 * A094743  Beginning with 2, increasing primes such that the sum of
 *          successive differences is also prime.
 * https://oeis.org/A094743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n = 1, otherwise the (n‑1)-th upper twin prime
 *            (prime p such that p-2 is also prime)
 * Range:     u8 n <= 18, u16 n <= 861, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ----- global static data ----- */
static seq_t *seq = NULL;      /* cache of sequence values           */
static idx_t seq_len = 0;      /* number of values stored            */
static idx_t seq_cap = 0;      /* allocated capacity (in seq_t units) */

static uint32_t *primes = NULL;    /* list of primes found so far        */
static size_t   prime_count = 0;   /* number of primes stored            */
static size_t   prime_cap   = 0;   /* allocated capacity (in uint32_t)   */

static uint32_t next_candidate = 3;/* next odd number to test           */

/* ----- helper functions ----- */
static bool is_prime(uint32_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

static void add_prime(uint32_t p)
{
    if (prime_count >= prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 32;
        primes = (uint32_t *)realloc(primes, prime_cap * sizeof(uint32_t));
    }
    primes[prime_count++] = p;
}

/* ----- sequence function ----- */
seq_t a(idx_t n)
{
    /* initialise cache and prime list */
    if (seq_len == 0) {
        seq_cap = 64;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)2;          /* a(1) = 2 */
        seq_len = 1;

        prime_cap = 32;
        primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
        primes[0] = 2;
        prime_count = 1;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        if (is_prime(next_candidate)) {
            add_prime(next_candidate);
            /* the candidate is a term iff candidate-2 is prime (except for 2 itself) */
            if (next_candidate > 2 && is_prime(next_candidate - 2)) {
                if (seq_len >= seq_cap) {
                    seq_cap *= 2;
                    seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
                }
                seq[seq_len++] = (seq_t)next_candidate;
            }
        }
        /* advance to next odd integer */
        next_candidate += 2;
        /* guard against integer overflow (very unlikely for the intended ranges) */
        if (next_candidate < 3) next_candidate = 3;
    }

    return seq[n - 1];
}
