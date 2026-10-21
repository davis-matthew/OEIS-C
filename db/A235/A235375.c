/*
 * A235375  Integers k such that k^2 is of form y^2 + y + x^2 + x for positive
 *          y, x.
 * https://oeis.org/A235375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 7487, u32 n <= 9441, u64 n <= 9441,
 *            u128 n <= 9441, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term (1‑based) of OEIS A235375:
   integers k such that k^2 is the sum of two oblong numbers. */

/* This code uses a simple prime sieve to test the condition
   that 8*t^2+1 has no prime ≡3 (mod4) raised to an odd power.
   The sequence contains k = 2*t for all t where this holds. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Static cache for the sequence. */
static seq_t *seq_cache = NULL;
static size_t seq_len  = 0;
static size_t seq_cap  = 0;

/* Primes used to test 8t^2+1. */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static bool     primes_ready = false;

/* Initialise a list of primes up to a fixed upper bound. */
static void init_primes(void)
{
    if (primes_ready) return;
    /* 200000 suffices for the range of n considered (t up to ~1e6). */
    const idx_t LIMIT = 200000;
    size_t sz = (size_t)(LIMIT + 1);
    bool *is_prime = (bool*)calloc(sz, sizeof(bool));
    if (!is_prime) abort();

    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;
    for (idx_t p = 2; p * p <= LIMIT; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= LIMIT; j += p)
                is_prime[j] = false;

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) abort();

    size_t pos = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) prime_list[pos++] = (uint64_t)i;

    prime_cnt = cnt;
    free(is_prime);
    primes_ready = true;
}

/* Test whether candidate t yields a valid k = 2*t. */
static bool t_is_valid(idx_t t)
{
    unsigned __int128 tmp = (unsigned __int128)8 * t * t + 1;
    if (tmp > (unsigned __int128)UINT64_MAX) return false; /* overflow guard */

    uint64_t N = (uint64_t)tmp;          /* N = 8*t^2 + 1  (odd) */

    init_primes();

    uint64_t remaining = N;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        if (p * p > remaining) break;
        if (remaining % p == 0)
        {
            int exp = 0;
            while (remaining % p == 0) { remaining /= p; ++exp; }
            if ((p & 3ULL) == 3ULL && (exp & 1)) return false;
        }
    }

    if (remaining > 1)                /* remaining prime > sqrt(N) */
    {
        if ((remaining & 3ULL) == 3ULL) return false;
    }
    return true;
}

/* Grow the cache array when needed. */
static void grow_sequence(void)
{
    size_t new_cap = seq_cap ? seq_cap << 1 : 128;
    seq_t *new_ptr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    if (seq_cache) {
        memcpy(new_ptr, seq_cache, seq_len * sizeof(seq_t));
        free(seq_cache);
    }
    seq_cache = new_ptr;
    seq_cap   = new_cap;
}

/* Sequence function as required. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined at offset 0 */

    static idx_t next_t = 1;              /* next candidate t to test */

    while (seq_len < (size_t)n)
    {
        if (seq_len >= seq_cap) grow_sequence();
        if (t_is_valid(next_t))
            seq_cache[seq_len++] = (seq_t)(2ULL * next_t);
        ++next_t;
    }
    return seq_cache[n - 1];
}
