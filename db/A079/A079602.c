/*
 * A079602  Greatest of smallest odd prime factors of all composite numbers
 *          between n-th prime and next prime.
 * https://oeis.org/A079602
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ spf_odd(m) : p_n < m < p_{n+1} , m composite } where
 *            p_k is the k‑th prime and spf_odd(m) is the smallest odd prime
 *            factor of m (m powers of 2 are skipped). This implementation
 *            builds a prime list and a smallest‑prime‑factor array
 *            incrementally, then scans the narrow interval between...
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---- prime generation ---------------------------------------------------- */
static size_t  primes_cap = 0;    /* allocated capacity of primes array      */
static idx_t  *primes     = NULL; /* array of primes (0‑based)              */
static size_t  primes_cnt = 0;    /* number of primes stored                */
static uint64_t next_candidate = 2; /* next integer to test for primality    */

static void ensure_primes(size_t needed_cnt)
{
    if (primes_cnt >= needed_cnt)
        return;               /* nothing to do                               */

    /* enlarge capacity if necessary */
    if (primes == NULL) {
        primes_cap = 32;
        primes = (idx_t *)malloc(primes_cap * sizeof(idx_t));
    }
    if (primes_cap < needed_cnt) {
        size_t new_cap = primes_cap ? primes_cap : 32;
        while (new_cap < needed_cnt)
            new_cap <<= 1;
        primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
        primes_cap = new_cap;
    }

    /* generate primes until we have enough */
    while (primes_cnt < needed_cnt) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > next_candidate)
                break;
            if (next_candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            primes[primes_cnt++] = next_candidate;
        ++next_candidate;
    }
}

/* ---- smallest prime factor array ---------------------------------------- */
static uint64_t *spf      = NULL; /* spf[i] = smallest prime factor of i         */
static size_t    spf_cap   = 0;   /* allocated capacity of spf array             */
static uint64_t  spf_limit = 0;   /* largest index for which spf is fully set    */

static void ensure_spf(uint64_t limit)
{
    if (limit <= spf_limit)
        return;                /* current spf data is sufficient              */

    if (spf == NULL) {
        spf_cap   = limit + 1;
        spf       = (uint64_t *)malloc(spf_cap * sizeof(uint64_t));
    } else if (spf_cap < limit + 1) {
        spf_cap = limit + 1;
        spf     = (uint64_t *)realloc(spf, spf_cap * sizeof(uint64_t));
    }

    /* initialise spf array */
    for (size_t i = 0; i <= limit; ++i)
        spf[i] = i;

    /* sieve to fill smallest prime factors */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {                     /* i is prime */
            for (uint64_t j = i; j <= limit; j += i) {
                if (spf[j] == j)
                    spf[j] = i;
            }
        }
    }

    spf_limit = limit;
}

/* ---- main sequence function ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;                   /* behaviour below offset is irrelevant     */

    /* we need primes up to the next (n+1‑th) prime */
    ensure_primes((size_t)(n + 1));

    uint64_t p_n    = (uint64_t)primes[n - 1];
    uint64_t p_next = (uint64_t)primes[n];

    /* make sure spf array covers the interval */
    ensure_spf(p_next - 1);

    uint64_t max_g = 0;
    for (uint64_t m = p_n + 1; m < p_next; ++m) {
        if (spf[m] == m)          /* m is prime, skip */
            continue;

        /* strip out all factors of 2 */
        uint64_t odd = m;
        while ((odd & 1) == 0)
            odd >>= 1;
        if (odd == 1)              /* power of two, no odd prime factor */
            continue;

        uint64_t spf_odd = spf[odd];
        if (spf_odd > max_g)
            max_g = spf_odd;
    }

    return (seq_t)max_g;
}
