/*
 * A141784  Primes of the form A141468(n)-n, where A141468(n)=n-th nonprime.
 * https://oeis.org/A141784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime that can be written as k-m where * k is the
 *            (m‑th) nonprime (including 0) and the difference k‑m is prime
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes primes by a fixed sieve and generates
 * the sequence on demand using a static cache. It supports arbitrary
 * idx_t and seq_t types provided by the build. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIEVE_MAX 4000000   /* Fixed sieve size (4 million). Enough for n≈10⁴ */

static bool *sieve = NULL;   /* prime sieve */
static size_t sieve_size = 0;

static seq_t    *cache  = NULL;    /* cache of generated a(n) values      */
static idx_t     cache_len = 0;     /* number of values stored in cache     */
static idx_t     cache_cap = 0;     /* cache capacity                       */

static idx_t     next_value = 0;        /* current integer being examined   */
static idx_t     next_nonprime_index = 0; /* count of nonprimes seen so far */

/* ------------------------------------------------------------- */
/*  Prime sieve initialisation                                   */
/* ------------------------------------------------------------- */
static void init_sieve(void)
{
    if (sieve != NULL) return;
    sieve_size = SIEVE_MAX;
    sieve = (bool *)malloc(sieve_size * sizeof(bool));
    for (size_t i = 0; i < sieve_size; ++i) sieve[i] = true;
    if (sieve_size > 0) sieve[0] = false;
    if (sieve_size > 1) sieve[1] = false;
    for (size_t p = 2; p * p < sieve_size; ++p) {
        if (sieve[p]) {
            for (size_t q = p * p; q < sieve_size; q += p)
                sieve[q] = false;
        }
    }
}

/* ------------------------------------------------------------- */
/*  Check primality of a number 0 <= n < sieve_size              */
/* ------------------------------------------------------------- */
static inline bool is_prime_small(uint64_t n)
{
    if (n < sieve_size) return sieve[(size_t)n];
    /* Fallback: trial division (used only if n >= sieve_size) */
    if (n < 2) return false;
    for (uint64_t p = 2; p * p <= n; ++p)
        if (n % p == 0) return false;
    return true;
}

/* ------------------------------------------------------------- */
/*  Ensure cache has at least n terms                           */
/* ------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;              /* already enough */

    /* Expand cache if necessary */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
        if (new_cap < n) new_cap = n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Generate terms until we reach the needed count */
    while (cache_len < n) {
        /* Process current value */
        bool prime_cur = is_prime_small((uint64_t)next_value);
        if (!prime_cur) {                    /* nonprime found */
            next_nonprime_index++;           /* increment nonprime counter */
            seq_t diff = (seq_t)next_value - (seq_t)next_nonprime_index;
            if (diff >= 2 && is_prime_small((uint64_t)diff)) {
                cache[cache_len++] = diff;
            }
        }
        next_value++;                        /* advance to next integer */
    }
}

/* ------------------------------------------------------------- */
/*  Main sequence function                                        */
/* ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0; return 0 */

    /* Initialise static data on first call */
    static bool init_done = false;
    if (!init_done) {
        init_done = true;
        init_sieve();

        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_value = 0;
        next_nonprime_index = 0;
    }

    ensure_cache(n);
    return cache[n - 1];
}
