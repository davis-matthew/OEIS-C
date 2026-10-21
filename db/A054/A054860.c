/*
 * A054860  Number of ways of writing 2n+1 as p + q + r where p, q, r are
 *          primes with p <= q <= r.
 * https://oeis.org/A054860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 187, u16 n <= 6517, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by sieving all primes up to 2n+1 and counting
   unordered triples (p,q,r) with p ≤ q ≤ r and p+q+r = 2n+1. */

/* The usual includes required for the helper functions supplied
   by the build system. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Local mutable state – allocated lazily on the first call          */
/* ------------------------------------------------------------------ */
static bool      initialized = false;   /* have we built a sieve yet?         */
static size_t    sieve_limit = 0;       /* largest number covered by the sieve */
static bool     *is_prime    = NULL;    /* primality table up to sieve_limit  */
static idx_t    *prime_list  = NULL;    /* list of primes ≤ sieve_limit      */
static size_t    prime_cnt   = 0;       /* number of entries in prime_list    */

/* Forward declarations */
static void recompute_sieve(idx_t limit);
static void ensure_sieve(idx_t max_needed);

/* ------------------------------------------------------------------ */
/* Sieve construction: builds is_prime[] up to ‘limit’ inclusive and
   fills prime_list with all primes ≤ limit.                          */
/* ------------------------------------------------------------------ */
static void recompute_sieve(idx_t limit)
{
    size_t l = (size_t)limit;

    /* Free old data if any */
    free(is_prime);
    free(prime_list);
    is_prime = NULL;
    prime_list = NULL;

    /* Allocate primality table: 0/1 for numbers 0..l */
    is_prime = (bool *)calloc(l + 1, sizeof(bool));
    if (!is_prime) return;                 /* allocation failure; caller handles */

    /* Sieve of Eratosthenes */
    for (size_t i = 2; i <= l; ++i) is_prime[i] = true;
    for (size_t i = 2; i * i <= l; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= l; j += i) is_prime[j] = false;
        }
    }

    /* Count primes and allocate list */
    prime_cnt = 0;
    for (size_t i = 2; i <= l; ++i) if (is_prime[i]) ++prime_cnt;
    if (prime_cnt == 0) return;            /* no primes found */

    prime_list = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    if (!prime_list) {                       /* allocation failure */
        prime_cnt = 0;
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= l; ++i) if (is_prime[i]) prime_list[idx++] = (idx_t)i;

    sieve_limit = l;
}

/* ------------------------------------------------------------------ */
/* Ensure a sieve covers all integers up to max_needed.               */
/* ------------------------------------------------------------------ */
static void ensure_sieve(idx_t max_needed)
{
    if (!initialized) {
        /* First time: build a modest default sieve. */
        recompute_sieve((idx_t)20002);   /* covers n up to about 5000 */
        initialized = true;
    }
    if ((size_t)max_needed <= sieve_limit) return;   /* already enough */

    /* Need a larger sieve: rebuild from scratch. */
    recompute_sieve(max_needed);
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* The odd number to be represented by p+q+r */
    idx_t N = 2 * n + 1;
    if (N < 7) return 0;               /* smallest representable odd is 7 */

    /* Make sure the sieve covers up to N. */
    ensure_sieve(N);

    seq_t count = 0;

    /* Iterate over primes p ≤ N. */
    for (size_t pi = 0; pi < prime_cnt && prime_list[pi] <= N; ++pi) {
        idx_t p = prime_list[pi];
        idx_t rest = N - p;                /* q + r = rest */

        /* q must be at least p and not exceed rest/2 (ensures q ≤ r). */
        for (size_t qi = pi; qi < prime_cnt; ++qi) {
            idx_t q = prime_list[qi];
            if (q > rest / 2) break;      /* q too large; r would be < q */
            idx_t r = rest - q;
            if (r < q) continue;          /* safety check */
            if (r <= sieve_limit && is_prime[r]) {
                ++count;
            }
        }
    }
    return count;
}
