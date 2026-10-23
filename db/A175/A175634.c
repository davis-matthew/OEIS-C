/*
 * A175634  Chen semiprimes: semiprimes m such that m+4 is either a prime or a
 *          semiprime.
 * https://oeis.org/A175634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth Chen semiprime (semiprime m such that m+4 is
 *            prime or semiprime). Computed by incremental sieving and
 *            semiprime marking
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Auxiliary functions provided by the build environment. */
extern bool is_prime_generic(seq_t n);
extern bool is_prime_small(seq_t n);
extern bool is_prime_big(seq_t n);
extern uint64_t _smallest_prime_factor(uint64_t n);

/* ---------- Internal state ---------- */
static seq_t *chen      = NULL;      /* stored Chen semiprimes */
static size_t chen_len  = 0;         /* number of elements stored */
static size_t chen_cap  = 0;         /* array capacity */

static idx_t last_processed = 0;     /* largest m examined so far */
static idx_t cur_limit      = 0;     /* current sieve size for is_prime_arr */

static uint8_t *is_prime_arr = NULL; /* sieve: 1 = prime, 0 = composite */
static uint8_t *is_semiprime = NULL; /* marks semiprimes up to cur_limit */
static uint32_t *primes   = NULL;    /* list of primes up to cur_limit */
static size_t  primes_cnt = 0;

/* ---------- Helpers ---------- */

/* Free existing sieve data. */
static void free_sieve(void)
{
    free(is_prime_arr);
    free(is_semiprime);
    free(primes);
    is_prime_arr = NULL;
    is_semiprime = NULL;
    primes       = NULL;
    primes_cnt   = 0;
}

/* ---------- Sieve construction ---------- */
static void rebuild(idx_t limit)
{
    /* free old data */
    free_sieve();

    /* allocate new sieve arrays */
    is_prime_arr = (uint8_t*)calloc((size_t)limit + 1, sizeof(uint8_t));
    is_semiprime = (uint8_t*)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!is_prime_arr || !is_semiprime) abort();

    /* Initialise primality table: 0,1 composite; others prime for now */
    for (idx_t i = 2; i <= limit; ++i) is_prime_arr[i] = 1u;
    is_prime_arr[0] = is_prime_arr[1] = 0u;

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; (i * i) <= limit; ++i)
        if (is_prime_arr[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime_arr[j] = 0u;

    /* Build list of primes */
    size_t estimated = (size_t)(limit / 10 + 16);
    primes = (uint32_t*)malloc(estimated * sizeof(uint32_t));
    if (!primes) abort();
    primes_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime_arr[i]) {
            if (primes_cnt == estimated) {
                estimated *= 2;
                primes = (uint32_t*)realloc(primes, estimated * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[primes_cnt++] = (uint32_t)i;
        }

    /* Mark semiprimes up to limit */
    for (size_t ii = 0; ii < primes_cnt; ++ii) {
        uint32_t p = primes[ii];
        for (size_t jj = ii; jj < primes_cnt; ++jj) {
            uint64_t prod = (uint64_t)p * (uint64_t)primes[jj];
            if (prod > (uint64_t)limit) break;
            is_semiprime[(idx_t)prod] = 1u;
        }
    }
}

/* ---------- Ensure enough Chen semiprimes have been generated ---------- */
static void ensure_terms(idx_t need)
{
    /* Grow sieve and generate until we have at least 'need' Chen semiprimes. */
    while (chen_len < need) {
        /* Determine next limit to sieve (grow roughly by factor 2). */
        idx_t next_limit = (cur_limit == 0) ? 1000 : cur_limit * 2;
        if (next_limit < cur_limit + 1000) next_limit = cur_limit + 1000;

        /* Also need data for m+4, so expand one more by 4. */
        idx_t sieve_limit = next_limit + 4;
        rebuild(sieve_limit);

        /* Scan new range for semiprimes. */
        for (idx_t m = last_processed + 1; m <= next_limit; ++m) {
            if (is_semiprime[m]) {
                idx_t mp4 = m + 4;
                bool ok = is_prime_arr[mp4] || is_semiprime[mp4];
                if (ok) {
                    /* Store Chen semiprime. */
                    if (chen_len == chen_cap) {
                        chen_cap = (chen_cap == 0) ? 16 : chen_cap * 2;
                        chen = (seq_t*)realloc(chen, chen_cap * sizeof(seq_t));
                        if (!chen) abort();
                    }
                    chen[chen_len++] = (seq_t)m;
                }
            }
        }

        last_processed = next_limit;
        cur_limit      = sieve_limit;
    }
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but safe fallback */
    ensure_terms(n);
    return chen[n - 1];           /* OEIS is 1‑based */
}
