/*
 * A062304  Primes of the form k - mu(k).
 * https://oeis.org/A062304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062304: primes of the form k - μ(k).
   Computes the sequence by sieving primes up to a fixed limit,
   evaluating μ(k) with a linear sieve, and collecting the primes
   k - μ(k).  The limit is chosen large enough to cover the first
   10 000 terms (≈ 10⁷).  The routine works for any build where
   seq_t and idx_t are integer types; for the arbitrary‑precision
   build seq_t behaves like a constructor from an unsigned integer. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static unsigned char  *is_prime = NULL;   /* 1‑bit sieve bitmap */
static signed char    *mu       = NULL;   /* Möbius values (-1,0,1) */
static uint32_t       *prime_tbl= NULL;   /* list of primes from sieve */
static size_t           prime_cnt = 0;
static size_t           prime_cap = 0;

static seq_t          *seq      = NULL;   /* table of primes k-μ(k) */
static size_t          seq_len  = 0;
static size_t          seq_cap  = 0;

/* ------------------------------------------------------------------ */
/* build a linear sieve of size limit, also computing μ */
static void build_sieve(size_t limit)
{
    free(is_prime);  free(mu);  free(prime_tbl);

    is_prime = (unsigned char*)malloc(limit + 1);
    mu       = (signed char*)  malloc(limit + 1);
    memset(is_prime, 1, limit + 1);
    is_prime[0] = is_prime[1] = 0;
    mu[0] = 0;  mu[1] = 1;

    prime_cap = limit / 10 + 1;            /* generous upper bound   */
    prime_tbl = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    prime_cnt = 0;

    for (size_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            prime_tbl[prime_cnt++] = (uint32_t)i;
            mu[i] = -1;
        }
        signed char mi = mu[i];
        for (size_t j = 0; j < prime_cnt; ++j) {
            size_t p = prime_tbl[j];
            size_t ip = i * p;
            if (ip > limit) break;
            is_prime[ip] = 0;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mi;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* generate the sequence up to the given limit (once). */
static void init_sequence(void)
{
    static bool initialized = false;
    if (initialized) return;

    /* limit carefully chosen to contain >10 000 terms */
    const size_t LIMIT = 10000000;
    build_sieve(LIMIT);

    seq_cap = 200000;   /* expected number of terms */
    seq_len = 0;
    seq = (seq_t*)calloc(seq_cap, sizeof(seq_t));

    for (size_t k = 2; k <= LIMIT; ++k) {
        /* μ(k) ∈ {–1,0,1} so k–μ(k) = k-1, k, or k+1 */
        int pk = (int)k - (int)mu[k];
        if (pk > 1 && pk <= LIMIT + 1 && is_prime[pk]) {
            if (seq_len == seq_cap) {
                seq_cap <<= 1;
                seq = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)pk;
        }
    }
    initialized = true;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    init_sequence();

    /* n is guaranteed to be ≤ seq_len for the expected range (≤10000) */
    return seq[n - 1];
}
