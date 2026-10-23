/*
 * A038946  Primes p such that 65 is not a square mod p.
 * https://oeis.org/A038946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038946: primes p such that 65 is NOT a square modulo p.
   a(n) = the nth smallest prime p with JacobiSymbol(65,p) == -1.
   Offsets: a(1)=3.  The function is memoised and works in any call order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/jacobi_symbol.h"

/* --- static storage for primes and sequence values ------------------- */
static uint64_t *prime_list   = NULL;   /* list of primes found so far          */
static size_t   prime_cap    = 0;        /* allocated capacity of prime_list     */
static size_t   prime_cnt    = 0;        /* number of primes stored             */

static seq_t *seq_cache      = NULL;   /* memoised sequence values            */
static size_t seq_cap        = 0;        /* allocated capacity of seq_cache     */
static size_t seq_len        = 0;        /* number of values stored             */

/* Next odd number to test for primality.  Starts at 3 after prime 2. */
static uint64_t next_candidate = 3;

/* --- helpers to grow dynamic arrays --------------------------------- */
static void ensure_prime_capacity(size_t new_cap)
{
    if (new_cap <= prime_cap) return;
    if (new_cap < prime_cap * 2) new_cap = prime_cap * 2;
    if (new_cap < 16)           new_cap = 16;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap  = new_cap;
}

static void ensure_seq_capacity(size_t new_cap)
{
    if (new_cap <= seq_cap) return;
    if (new_cap < seq_cap * 2) new_cap = seq_cap * 2;
    if (new_cap < 16)           new_cap = 16;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    seq_cap   = new_cap;
}

/* --- main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                            /* undefined offset */

    /* Initialise static storage on first call. */
    if (!prime_list) {
        ensure_prime_capacity(32);
        prime_list[0] = 2;   /* prime 2 is used for trial division */
        prime_cnt    = 1;
        next_candidate = 3;
    }
    if (!seq_cache) ensure_seq_capacity(32);

    /* Return memoised value if already computed. */
    if (seq_len >= (size_t)n) return seq_cache[n - 1];

    /* Search for new primes until we have at least n sequence values. */
    while (seq_len < (size_t)n) {
        /* --- find next prime ------------------------------------- */
        uint64_t cand = next_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* Store new prime for future trial divisions. */
            ensure_prime_capacity(prime_cnt + 1);
            prime_list[prime_cnt++] = cand;

            /* Jacobi symbol (65|cand).  For primes dividing 65 it is 0. */
            if (jacobi_symbol(65, cand) == -1) {
                ensure_seq_capacity(seq_len + 1);
                seq_cache[seq_len++] = (seq_t)cand;
            }
        }

        /* Advance to the next odd candidate. */
        next_candidate = cand + 2;
    }

    return seq_cache[n - 1];
}
