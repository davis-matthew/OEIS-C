/*
 * A088710  Numbers m which are a product of two primes j and k such that
 *          m-j-k is prime.
 * https://oeis.org/A088710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th integer m such that m = p·q (p,q prime,
 *            possibly equal) and m – p – q is prime. Implementation: a sieve
 *            of Eratosthenes generates primes up to a dynamic limit; we then
 *            iterate integers, check the semiprime condition using the
 *            smallest prime factor array, and collect those that sa...
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Static arrays for caching the sequence and the prime sieve */
static seq_t *seq_vals   = NULL;   /* stored sequence values */
static size_t seq_len    = 0;       /* number of terms currently stored */
static size_t seq_cap    = 0;       /* capacity of seq_vals */

static uint32_t *spfs    = NULL;   /* smallest prime factor for each number up to current sieve limit */
static bool     *is_prime = NULL;  /* primality flag */
static uint32_t *primes   = NULL;   /* list of primes discovered by the sieve */
static size_t    primes_len  = 0;
static size_t    primes_cap  = 0;

static uint32_t sieve_limit = 0;     /* current size of the sieve (inclusive) */
static uint32_t current_m   = 0;     /* next integer to test for membership in the sequence */

/* Append a new value to the cached sequence */
static void append_seq_val(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq_t *tmp = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        seq_vals = tmp;
        seq_cap  = new_cap;
    }
    seq_vals[seq_len++] = v;
}

/* Add a prime to the primes list */
static void add_prime(uint32_t p)
{
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 64;
        uint32_t *tmp = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) exit(1);
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

/* Build a linear sieve up to 'limit' (inclusive) */
static void build_sieve(uint32_t limit)
{
    if (limit < 2) limit = 2;

    /* Allocate or reallocate the sieve arrays */
    if (spfs) free(spfs);
    if (is_prime) free(is_prime);
    spfs      = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    is_prime  = (bool*)   calloc(limit + 1, sizeof(bool));
    if (!spfs || !is_prime) exit(1);

    /* Allocate prime list */
    if (primes) free(primes);
    primes_cap = limit / 10 + 10;
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(1);
    primes_len = 0;

    /* Linear sieve algorithm */
    for (uint32_t i = 2; i <= limit; ++i) {
        if (spfs[i] == 0) {
            spfs[i] = i;
            add_prime(i);
            is_prime[i] = true;
        }
        for (size_t j = 0; j < primes_len; ++j) {
            uint32_t p = primes[j];
            uint64_t prod = (uint64_t)i * p;
            if (p > spfs[i] || prod > limit) break;
            spfs[(uint32_t)prod] = p;
        }
    }

    sieve_limit = limit;
}

/* Extend the memoised sequence until it contains at least 'need' terms */
static void extend_terms(idx_t need)
{
    if (seq_len >= (size_t)need) return;

    /* Lazily initialise the first test value */
    if (current_m == 0) current_m = 4; /* 2*2 is the smallest semiprime */

    /* Initialise the sieve if it doesn't exist yet */
    if (sieve_limit == 0) {
        uint32_t est = (uint32_t)(need * 200 + 100);  /* heuristic upper bound */
        if (est < 20) est = 20;
        build_sieve(est);
    }

    while (seq_len < (size_t)need) {
        /* Extend the sieve when we run out of range */
        if (current_m > sieve_limit) {
            uint32_t new_limit = sieve_limit * 2;
            if (new_limit < current_m) new_limit = current_m * 2;
            build_sieve(new_limit);
        }

        uint32_t m = current_m;
        uint32_t p = spfs[m];
        if (p != 0 && p != m) { /* m is composite */
            uint32_t q = m / p;
            if (q <= sieve_limit && is_prime[q]) { /* exactly two prime factors */
                uint32_t r = m - p - q;
                if (r >= 2 && r <= sieve_limit && is_prime[r]) {
                    append_seq_val((seq_t)m);
                }
            }
        }
        ++current_m;
    }
}

/* Public interface required by the test harness */
seq_t a(idx_t n)
{
    extend_terms(n);
    return seq_vals[n - 1];
}
