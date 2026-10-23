/*
 * A100952  Numbers that cannot be written as p*q+r with three distinct primes
 *          p, q and r.
 * https://oeis.org/A100952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 21, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   is_prime, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A100952: numbers that cannot be written as
   p*q + r with three distinct primes p, q and r.        */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"

/* ---------- prime sieve ------------------------------------- */
#define SIEVE_LIMIT 2000000  /* sufficient for a(n) up to n ≈ 10 000 */

/* dynamic array of primes, filled on first call */
static uint64_t *prime_buf = NULL;
static size_t prime_count = 0;

/* generate all primes ≤ SIEVE_LIMIT */
static void generate_primes(void)
{
    if (prime_buf != NULL) return;

    char *is_prime = (char *)malloc(SIEVE_LIMIT + 1);
    if (!is_prime) abort();

    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= SIEVE_LIMIT; ++i)
    {
        if (is_prime[i])
            for (uint64_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = 0;
    }

    /* count primes */
    prime_count = 0;
    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++prime_count;

    prime_buf = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_buf) abort();

    size_t pos = 0;
    for (uint64_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (is_prime[i]) prime_buf[pos++] = i;

    free(is_prime);
}

/* ---------- check whether X has a representation p*q+r ---------- */
static bool has_representation(seq_t X)
{
    if (X < 11) return false;            /* all numbers < 11 are non‑representable */

    uint64_t x   = (uint64_t)X;
    uint64_t root = (uint64_t)isqrt(X); /* floor(sqrt(x)) */

    /* iterate over pairs p < q of distinct primes */
    for (size_t i = 0; i < prime_count && prime_buf[i] <= root; ++i)
    {
        uint64_t p = prime_buf[i];
        for (size_t j = i + 1; j < prime_count; ++j)
        {
            uint64_t q = prime_buf[j];
            uint64_t prod = p * q;
            if (prod > x - 2) break;    /* r ≥ 2 */

            uint64_t r = x - prod;
            if (r == p || r == q) continue;   /* primes must be distinct */

            if (is_prime(r)) return true;
        }
    }
    return false;
}

/* ---------- memoized sequence storage -------------------------- */
static seq_t *seq_buf = NULL;
static size_t seq_len = 0;       /* number of terms stored */
static size_t seq_cap = 0;       /* capacity of seq_buf */

/* ensure capacity for at least 'needed' elements */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq_buf = (seq_t *)realloc(seq_buf, new_cap * sizeof(seq_t));
    if (!seq_buf) abort();
    seq_cap = new_cap;
}

/* next integer to examine.  kept across calls so that the search
   continues from where it left off.  init to 1. */
static seq_t next_candidate = 1;

/* ---------- main function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1 */

    /* generate primes once */
    if (prime_buf == NULL) generate_primes();

    /* extend the stored sequence until we have at least n terms */
    while (seq_len < (size_t)n)
    {
        seq_t x = next_candidate;
        /* test x for representability */
        if (!has_representation(x))
        {
            ensure_seq_capacity(seq_len + 1);
            seq_buf[seq_len++] = x;
        }
        ++next_candidate;
    }

    return seq_buf[n - 1];
}
