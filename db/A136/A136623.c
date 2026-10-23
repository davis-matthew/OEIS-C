/*
 * A136623  a(n) is the largest prime power that is <=n and is coprime to n.
 * https://oeis.org/A136623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the largest prime power <= n that is coprime to n. */

/* ----------------------------------------------------------------------
 * Utility: build a list of primes ≤ limit using a simple sieve of Eratosthenes.
 * ---------------------------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out)
{
    if (limit < 2) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    /* Allocate the sieve array */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool*)calloc(sz, sizeof(bool));
    if (!is_prime) {
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    /* Mark all numbers as prime initially */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;

    /* Sieve */
    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Allocate and fill primes array */
    uint64_t *pr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!pr) {
        free(is_prime);
        *primes_out = NULL;
        *cnt_out = 0;
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[idx++] = i;

    free(is_prime);

    *primes_out = pr;
    *cnt_out = cnt;
}

/* Cached prime table */
static uint64_t *prime_tbl = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure that prime_tbl contains all primes ≤ n. */
static void ensure_primes(idx_t n)
{
    if (n <= prime_limit) return;
    if (prime_tbl) free(prime_tbl);
    build_primes_upto(n, &prime_tbl, &prime_cnt);
    prime_limit = n;
}

/* ----------------------------------------------------------------------
 * Main function: a(n)
 * ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)1;  /* for completeness, though n≥1 per spec */

    ensure_primes(n);

    seq_t best = (seq_t)1;      /* fallback if no larger prime power found */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if ((seq_t)p > n) break;          /* primes are increasing */
        if ((n % (idx_t)p) == 0) continue;/* skip primes that divide n */

        /* Compute the largest power of p that does not exceed n */
        seq_t val = (seq_t)p;
        /* Use division by p to avoid overflow */
        while (val <= (seq_t)(n / (idx_t)p)) {
            val *= (seq_t)p;
        }

        if (val > best) best = val;
    }

    return best;
}
