/*
 * A065300  Numbers k such that the sum of divisors of k is a squarefree
 *          number.
 * https://oeis.org/A065300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 2190, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A065300: numbers k such that sigma(k) is squarefree.
   The function builds the sequence incrementally using an
   incremental search.  All results are memoised so subsequent
   calls in any order remain fast. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_squarefree.h"

static uint64_t *primes = NULL;       /* array of primes */
static size_t   primes_cnt = 0;      /* number of primes */
static bool     primes_ok = false;   /* primes initialised? */

/* ------------------------------------------------------------------ */
/* --- Helper: build a list of primes up to a fixed limit.           */
/* ------------------------------------------------------------------ */
static void init_primes(void)
{
    if (primes_ok) return;

    /* A limit that comfortably contains sqrt(k) for k up to a few
       million (more than enough for n up to about ten thousand). */
    const idx_t LIMIT = 1000000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) return;   /* should never happen in test env */

    for (idx_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes */
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) ++primes_cnt;

    primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes) { free(is_prime); return; }

    size_t p = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i]) primes[p++] = (uint64_t)i;

    free(is_prime);
    primes_ok = true;
}

/* ------------------------------------------------------------------ */
/* --- Helper: compute sigma(k) using the global prime list.           */
/* ------------------------------------------------------------------ */
static unsigned long long calc_sigma(idx_t k)
{
    unsigned long long sigma = 1;
    idx_t n = k;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if ((idx_t)p * p > n) break;
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) { n /= p; ++e; }

            unsigned long long term = 1;
            unsigned long long pow = 1;
            for (size_t j = 0; j < (size_t)e; ++j) {
                pow *= p;
                term += pow;          /* (p^{e+1}-1)/(p-1) */
            }
            sigma *= term;
        }
    }
    if (n > 1)  /* remaining prime factor */
        sigma *= (unsigned long long)(n) + 1;

    return sigma;
}

/* ------------------------------------------------------------------ */
/* --- Memoised incremental search for A065300                       */
/* ------------------------------------------------------------------ */
static seq_t *cache  = NULL;   /* storage of found terms */
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static idx_t   cur_k      = 1;  /* next k to test */
static idx_t   found_cnt  = 0;  /* number of terms stored */

static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* --- Public function: a(n)                                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of bounds, but harmless */

    /* initialise primes once */
    if (!primes_ok) init_primes();

    /* fast reply if already computed */
    if (found_cnt >= n) return cache[n - 1];

    /* generate additional terms until we reach n */
    while (found_cnt < n) {
        unsigned long long sig = calc_sigma(cur_k);
        if (is_squarefree((seq_t)sig)) {
            /* store the new term */
            ensure_cache_cap(found_cnt + 1);
            cache[found_cnt] = (seq_t)cur_k;
            ++found_cnt;
        }
        ++cur_k;
    }
    return cache[n - 1];
}
