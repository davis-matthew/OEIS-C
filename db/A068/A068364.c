/*
 * A068364  Primes p such that there exists k such that p = prime(k) +
 *          prime(k+2) + prime(k+4) + prime(k+6) + prime(k+8).
 * https://oeis.org/A068364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 313, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes that are the sum of 5 alternate primes.  The function
   generates all primes up to a fixed upper bound using a simple
   sieve, then scans sums of the form
        p_k + p_{k+2} + p_{k+4} + p_{k+6} + p_{k+8}
   and collects those that are prime.  Results are memoised so that
   calls to a(idx_t n) need not be in increasing order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* upper bound for the sieve (primes up to this value are generated) */
static const uint32_t SIEVE_LIMIT = 50000000u;   /* 50 million */

/* state for the sieve and prime list */
static bool init_done = false;
static uint8_t *is_comp = NULL;          /* 0 = prime, 1 = composite */
static uint32_t *primes = NULL;          /* list of primes */
static size_t primes_count = 0;

/* memoised sequence values */
static seq_t *results = NULL;            /* collected terms of the sequence */
static size_t results_size = 0;
static size_t results_cap = 0;

/* next index k whose neighbourhood will be examined */
static size_t next_k = 0;

/* ------------------------------------------------------------------- */
/*  Simple sieve of Eratosthenes to fill is_comp and primes array      */
/* ------------------------------------------------------------------- */
static void init_sieve(void)
{
    /* allocate array for composite markers */
    is_comp = (uint8_t *)calloc(SIEVE_LIMIT + 1, 1);
    if (!is_comp) abort();

    is_comp[0] = is_comp[1] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = 1;
        }
    }

    /* estimate number of primes up to SIEVE_LIMIT (~0.5 * n / ln n) */
    size_t est = (size_t)(SIEVE_LIMIT / 2);
    primes = (uint32_t *)malloc(est * sizeof(uint32_t));
    if (!primes) abort();

    primes_count = 0;
    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            if (primes_count == est) {   /* enlarge if estimate was low */
                est += est / 2 + 1024;
                uint32_t *tmp = (uint32_t *)realloc(primes, est * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_count++] = i;
        }
    }

    init_done = true;
}

/* ------------------------------------------------------------------- */
/*  Helper: append a value to the results array, growing it if needed  */
/* ------------------------------------------------------------------- */
static void append_result(seq_t v)
{
    if (results_size == results_cap) {
        size_t new_cap = results_cap ? results_cap * 2 : 32;
        seq_t *tmp = (seq_t *)realloc(results, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        results = tmp;
        results_cap = new_cap;
    }
    results[results_size++] = v;
}

/* ------------------------------------------------------------------- */
/*  Ensure that at least 'need' terms have been computed                */
/* ------------------------------------------------------------------- */
static void ensure_nterms(idx_t need)
{
    while (results_size < need) {
        /* need to have primes up to index next_k+8 */
        if (next_k + 8 >= primes_count)
            abort();               /* should not happen with chosen limit */

        /* sum the 5 alternate primes */
        seq_t sum = 0;
        sum += (seq_t)primes[next_k];
        sum += (seq_t)primes[next_k + 2];
        sum += (seq_t)primes[next_k + 4];
        sum += (seq_t)primes[next_k + 6];
        sum += (seq_t)primes[next_k + 8];

        /* if sum is within sieve limit and prime, record it */
        if (sum <= (seq_t)SIEVE_LIMIT && !is_comp[(uint32_t)sum])
            append_result(sum);

        ++next_k;
    }
}

/* ------------------------------------------------------------------- */
/*  Public function required by the framework                          */
/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, return 0 */
    if (!init_done) init_sieve();
    ensure_nterms(n);
    return results[n - 1];
}
