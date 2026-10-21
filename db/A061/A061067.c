/*
 * A061067  m-th prime prime(m) is included iff prime(m) + m is also prime.
 * https://oeis.org/A061067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 663, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A061067: a(n) = the n‑th prime p_k such that p_k + k is also prime
    (offset 0).  The implementation uses a static list of primes and
    accumulates qualifying primes until the requested index is available. */

#include <stdint.h>
#include <stdbool.h>

/* Maximum numbers expected for typical test limits */
#define MAX_PRIMES 400000   /* enough for n up to ~10000 */
#define MAX_SEQ    10001    /* holds the first 10001 terms */

/* Storage for primes (1‑based index during generation) */
static idx_t primes[MAX_PRIMES];
static idx_t prime_cnt = 0;

/* Storage for sequence values */
static seq_t seq_vals[MAX_SEQ];
static idx_t seq_cnt = 0;

/* Simple primality test using the primes already found */
static bool is_prime_candidate(idx_t x)
{
    if (x < 2) return false;
    for (idx_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Ensure the sequence is generated up to index n (inclusive). */
static void generate_upto(idx_t n)
{
    while (seq_cnt <= n)
    {
        /* Find next prime */
        idx_t cand = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;
        while (!is_prime_candidate(cand))
            ++cand;

        /* Store the new prime */
        if (prime_cnt >= MAX_PRIMES)
            abort();          /* safety guard – should not happen for tests */
        primes[prime_cnt++] = cand;

        /* Check the condition p_k + k is prime */
        idx_t k   = prime_cnt;          /* 1‑based index of this prime */
        idx_t test_val = cand + k;
        if (is_prime_candidate(test_val))
        {
            if (seq_cnt >= MAX_SEQ)
                abort();          /* safety guard */
            seq_vals[seq_cnt++] = (seq_t)cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n >= MAX_SEQ) abort();   /* safety guard for out‑of‑bounds indices */
    generate_upto(n);
    return seq_vals[n];
}
