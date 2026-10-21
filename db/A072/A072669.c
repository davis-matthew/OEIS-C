/*
 * A072669  Primes of the form prime(x) + prime(x+1) - 1.
 * https://oeis.org/A072669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 906, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A072669: primes of the form prime(m)+prime(m+1)-1
   (the sum of two consecutive primes minus one).
   The code generates primes on demand, checks each sum for primality,
   and caches all terms found, so repeated calls are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* ------------------------------------------------------------------ */
/* Prime generation helpers                                            */
/* ------------------------------------------------------------------ */

static uint64_t *prime_list   = NULL;   /* generated primes            */
static size_t   prime_cnt    = 0;       /* number of primes stored     */
static size_t   prime_cap    = 0;       /* capacity of prime_list      */
static uint64_t next_candidate = 2;     /* candidate for next prime    */

/* Ensure prime_list has capacity at least cap. */
static void ensure_prime_cap(size_t cap)
{
    if (prime_cap < cap) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 32;
        if (new_cap < cap) new_cap = cap;
        uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!tmp) abort();                 /* out of memory */
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

/* Generate a single new prime and append to prime_list. */
static void generate_next_prime(void)
{
    for (;;) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            __uint128_t sq = (__uint128_t)p * p;
            if (sq > next_candidate) break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            ensure_prime_cap(prime_cnt + 1);
            prime_list[prime_cnt++] = next_candidate;
        }
        ++next_candidate;
        if (is_prime) break;
    }
}

/* Ensure that we have at least required+1 primes (indices 0..required). */
static void ensure_primes_upto(size_t required)
{
    while (prime_cnt <= required)
        generate_next_prime();
}

/* Test if x (uint64_t) is prime using the generated prime list.  */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        __uint128_t sq = (__uint128_t)p * p;
        if (sq > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Sequence cache helpers                                             */
/* ------------------------------------------------------------------ */

static seq_t *seq_cache = NULL;   /* cached sequence values         */
static size_t seq_cap   = 0;      /* capacity of seq_cache          */
static size_t seq_len   = 0;      /* current number of terms cached */

/* Ensure seq_cache has capacity at least cap. */
static void ensure_seq_cap(size_t cap)
{
    if (seq_cap < cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 32;
        if (new_cap < cap) new_cap = cap;
        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                  /* out of memory */
        seq_cache = tmp;
        seq_cap   = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the harness                             */
/* ------------------------------------------------------------------ */

/* a(n) returns the n-th term of the sequence (1‑based index).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined outside OEIS offset */

    /* We may be called in any order, so run until we have at least n terms. */
    static idx_t m_checked = 0;               /* next m index to examine */

    while (seq_len < (size_t)n) {
        /* Ensure we have primes for indices m_checked and m_checked+1. */
        ensure_primes_upto((size_t)(m_checked + 1));

        /* Compute sum = prime(m) + prime(m+1) - 1. */
        uint64_t sum = prime_list[m_checked] + prime_list[m_checked + 1] - 1ULL;

        /* Check primality of sum. */
        if (is_prime_uint64(sum)) {
            ensure_seq_cap(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)sum;
        }

        ++m_checked;
    }

    return seq_cache[n - 1];
}
