/*
 * A171836  a(n) = A002476(A002476(n)).
 * https://oeis.org/A171836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A002476(A002476(n))
 * Range:     u8 n <= 3, u16 n <= 224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A002476 is the list of primes congruent to 1 (mod 6) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static seq_t *primes1 = NULL;          /* array of primes  ≡ 1 (mod 6) */
static size_t primes1_size = 0;        /* number of stored primes */
static size_t primes1_cap  = 0;        /* allocated capacity */
static idx_t  max_index_needed = 0;    /* largest index we have ensured */
static idx_t  sieve_limit     = 0;     /* maximum prime value we have sieved */

/* Estimate an upper bound for the value of the n‑th prime ≡ 1 (mod 6) */
static idx_t estimate_limit_for_n(idx_t n)
{
    if (n < 6) return 20;                          /* small n */
    double dn = (double)n;
    /* Approximates the n‑th prime: p_n ≈ n (log n + log log n)   */
    double est = 2.0 * dn * (log(dn) + log(log(dn)));   /* 1‑mod‑6 primes are about half of all primes */
    return (idx_t)(est + 100.0);                 /* add a safety margin */
}

/* Build primes ≡ 1 (mod 6) up to 'limit' (inclusive).  The array
   'primes1' will hold all such primes ≤ limit.               */
static void build_primes1_upto(idx_t limit)
{
    /* Sieve of Eratosthenes */
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return;            /* allocation failed */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t mult = p * p; mult <= limit; mult += p)
                is_prime[mult] = false;
        }
    }

    /* Count how many primes 1 mod 6 we will store */
    size_t count = 0;
    for (idx_t i = 7; i <= limit; ++i) {   /* 7 is the first such prime */
        if (is_prime[i] && (i % 6 == 1))
            ++count;
    }

    /* Reallocate array if needed */
    if (count > primes1_cap) {
        size_t new_cap = count * 2;          /* some slack */
        seq_t *tmp = (seq_t *)realloc(primes1, new_cap * sizeof(seq_t));
        if (!tmp) { free(is_prime); return; }
        primes1      = tmp;
        primes1_cap  = new_cap;
    }

    /* Store the primes */
    primes1_size = 0;
    for (idx_t i = 7; i <= limit; ++i) {
        if (is_prime[i] && (i % 6 == 1))
            primes1[primes1_size++] = (seq_t)i;
    }
    sieve_limit = limit;
    free(is_prime);
}

/* Ensure that we have at least 'idx_needed' primes stored in primes1. */
static void ensure_primes_upto_idx(idx_t idx_needed)
{
    if (idx_needed <= max_index_needed) return;   /* already enough */

    /* Determine the value that is at position idx_needed (1‑based) */
    idx_t limit = estimate_limit_for_n(idx_needed);
    if (limit <= sieve_limit) limit = sieve_limit;   /* keep existing limit if larger */

    build_primes1_upto(limit);
    max_index_needed = idx_needed;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe */

    /* Step 1: find the n‑th prime ≡ 1 (mod 6) */
    ensure_primes_upto_idx(n);
    seq_t first = primes1[n - 1];        /* 0‑based */

    /* Step 2: interpret 'first' as an index and find that prime */
    ensure_primes_upto_idx((idx_t)first);
    seq_t second = primes1[(idx_t)first - 1];   /* first is 1‑based ((idx_t)first) */

    return second;
}
