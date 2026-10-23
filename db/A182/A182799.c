/*
 * A182799  Positions of primes in A167171.
 * https://oeis.org/A182799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Positions of primes in A167171.
   Formula: a(n) = n + #{p*q <= p_n : p < q are primes}
   where p_n is the n-th prime.
   The result is the index (starting from 1) of the n-th prime
   inside the sorted set of primes and distinct‑prime semiprimes. */
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static bool initialized = false;
static idx_t *primes = NULL;     /* list of primes up to LIMIT */
static size_t primes_count = 0;  /* number of primes in the list */
static unsigned *pi = NULL;      /* pi[i] = #{primes <= i} */
static idx_t LIMIT = 0;

/* Simple linear sieve up to LIMIT.  Adjust LIMIT for desired n.
   This function assumes that LIMIT is large enough to contain at least
   the first 10 000 primes (≈ 200 000). */
static void init_sieve(void) {
    if (initialized) return;
    /* choose a generous limit; 2000000 is more than enough for 10000 primes */
    LIMIT = 2000000;
    bool *is_prime = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!is_prime) return; /* fallback – should not happen for typical builds */
    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }

    /* Count primes and build pi array */
    pi = (unsigned*)malloc((LIMIT + 1) * sizeof(unsigned));
    primes = (idx_t*)malloc((LIMIT / 10) * sizeof(idx_t)); /* rough estimate */
    if (!pi || !primes) return; /* fallback */

    primes_count = 0;
    unsigned count = 0;
    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_count >= LIMIT / 10) {
                /* grow primes array if necessary */
                size_t new_cap = (LIMIT / 10) * 2;
                idx_t *tmp = (idx_t*)realloc(primes, new_cap * sizeof(idx_t));
                if (!tmp) break;
                primes = tmp;
            }
            primes[primes_count++] = i;
            ++count;
        }
        pi[i] = count;
    }
    /* free temporary sieve */
    free(is_prime);
    initialized = true;
}

seq_t a(idx_t n) {
    init_sieve();
    if (n == 0 || primes_count < (size_t)n) {
        return (seq_t)0; /* out of range – undefined for this build */
    }
    idx_t p_n = primes[(size_t)(n - 1)]; /* p_n is the n-th prime */
    /* count semiprimes p*q <= p_n with p<q */
    idx_t cnt = 0;
    for (size_t i = 0; i < primes_count; ++i) {
        idx_t p = primes[i];
        if (p * p > p_n) break; /* p <= sqrt(p_n) */
        idx_t r = p_n / p;      /* maximum q allowed */
        if (r <= p) continue;   /* need q > p */
        /* primes in (p, r] */
        /* pi array is defined up to LIMIT >= p_n */
        cnt += (idx_t)(pi[r] - pi[p]);
    }
    return (seq_t)n + (seq_t)cnt;
}
