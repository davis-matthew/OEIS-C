/*
 * A082049  Least composite number greater than n-th composite number having
 *          greater smallest prime factor than that of n-th composite number.
 * https://oeis.org/A082049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A082049:  Least composite number greater than the n-th
    composite number having a greater smallest prime factor
    than that of the n-th composite number.
    The function keeps a dynamic sieve of smallest prime
    factors and a list of composite numbers and searches
    for the required value.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* external linkage required by the harness */
seq_t a(idx_t n);

/* ---------- internals ------------------------------------- */

static uint32_t *spf      = NULL;  /* smallest prime factor table */
static uint32_t *primes   = NULL;  /* primes up to current_limit   */
static size_t   primes_cnt  = 0;
static size_t   primes_cap  = 0;
static size_t   current_limit = 0;  /* size of spf table            */

static uint32_t *composites = NULL; /* list of composite numbers   */
static size_t   composites_cnt = 0;
static size_t   composites_cap = 0;
static uint32_t next_candidate = 4;  /* next number to test          */

/* build a sieve up to new_limit (inclusive) */
static void build_sieve(uint32_t new_limit)
{
    free(spf);
    free(primes);
    spf = (uint32_t *)calloc(new_limit + 1, sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);

    primes_cap = new_limit / 10 + 1;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    primes_cnt = 0;

    for (uint32_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= new_limit) {
                for (uint32_t j = i * i; j <= new_limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    current_limit = new_limit;
}

/* ensure we have a sieve up to at least 'need' */
static void ensure_limit(uint32_t need)
{
    if (current_limit >= need) return;
    uint32_t new_limit = (current_limit > 0) ? current_limit << 1 : 128;
    while (new_limit < need)
        new_limit <<= 1;
    build_sieve(new_limit);
}

/* generate composites until we have at least 'need' entries */
static void ensure_composites(size_t need)
{
    if (composites_cnt >= need) return;

    if (composites_cap == 0) {
        composites_cap = 32;
        composites = (uint32_t *)malloc(composites_cap * sizeof(uint32_t));
        if (!composites) exit(EXIT_FAILURE);
    }

    for (; composites_cnt < need; ) {
        if (next_candidate > current_limit) ensure_limit(next_candidate);

        if (next_candidate >= 2 && spf[next_candidate] < next_candidate) {
            if (composites_cnt == composites_cap) {
                composites_cap <<= 1;
                composites = (uint32_t *)realloc(composites, composites_cap * sizeof(uint32_t));
                if (!composites) exit(EXIT_FAILURE);
            }
            composites[composites_cnt++] = next_candidate;
        }
        ++next_candidate;
    }
}

/* ---------- public function --------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, safe fallback */
        return (seq_t)0;

    /* Ensure we have a sieve covering the required numbers. */
    /* 1) Get the n-th composite number. */
    ensure_composites((size_t)n);
    uint32_t cn = composites[(size_t)(n - 1)];          /* n-th composite   */
    uint32_t spf_c = spf[cn];                          /* its smallest prime factor */

    /* 2) Search for the least composite > cn with larger spf. */
    uint32_t m = cn + 1;
    for (;;) {
        if (m > current_limit) ensure_limit(m);

        if (m >= 2 && spf[m] < m && spf[m] > spf_c)
            return (seq_t)m;

        ++m;
    }
}
