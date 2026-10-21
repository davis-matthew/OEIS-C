/*
 * A229064  Lesser of Fermi-Dirac twin primes: both a(n)(>=5) and a(n)+2 are
 *          in A050376.
 * https://oeis.org/A229064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A229064 – Lesser of Fermi‑Dirac twin primes.
 *
 *  The sequence is defined as the lesser of every pair of consecutive
 *  Fermi‑Dirac numbers (numbers of the form p^(2^k) with p prime and k≥0)
 *  that differ by 2.  In other words, a(n) is the smallest member of each
 *  pair (q, q+2) where both q and q+2 belong to A050376.
 *
 *  Implementation: a pre‑computed table of all such pairs is built once
 *  by sieving primes up to a fixed limit (10 000 000).  All
 *  Fermi‑Dirac numbers ≤ limit are marked, then all pairs with a
 *  difference of 2 are collected in order.  The table is memoised so
 *  that any later call for arbitrary n (not necessarily increasing)
 *  returns the correct value in O(1) time after the initial build.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- constants -----------------------------------------------------------*/
static const idx_t MAX_LIMIT = 10000000ULL;   /* upper bound for cache build */

/* --- static storage -----------------------------------------------------*/
static seq_t  *cache      = NULL;   /* array of a(n) values          */
static size_t cache_len  = 0;        /* number of stored terms        */
static size_t cache_cap  = 0;        /* capacity of cache array       */
static bool    cache_ready = false;  /* indicates precomputation done */

/* tables for stepping through the sieve/out‐placement */
static uint8_t *is_fd   = NULL;   /* Fermi‑Dirac flag array         */
static uint8_t *is_composite = NULL;  /* composite flag array       */
static uint32_t *primes = NULL;  /* list of primes up to MAX_LIMIT */
static size_t   primes_len = 0;
static size_t   primes_cap = 0;

/* --- helper: grow the cache array -------------------------------------*/
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ?: 256;
    while (cap < new_cap) cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = cap;
}

/* --- helper: ensure a prime list up to MAX_LIMIT ----------------------*/
static void build_primes(void)
{
    is_composite = (uint8_t *)calloc(MAX_LIMIT + 1, 1);
    if (!is_composite) exit(EXIT_FAILURE);

    primes_cap = 1000000;  /* initial allocation */
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    for (idx_t i = 2; i * i <= MAX_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= MAX_LIMIT; j += i)
                is_composite[j] = 1;
        }
    }
    for (idx_t i = 2; i <= MAX_LIMIT; ++i) {
        if (!is_composite[i]) {
            if (primes_len == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) exit(EXIT_FAILURE);
                primes = tmp;
            }
            primes[primes_len++] = (uint32_t)i;
        }
    }
}

/* --- helper: mark all Fermi‑Dirac numbers ≤ MAX_LIMIT ------------------*/
static void mark_fd_numbers(void)
{
    is_fd = (uint8_t *)calloc(MAX_LIMIT + 1, 1);
    if (!is_fd) exit(EXIT_FAILURE);

    /* exponent 1 (p) */
    for (size_t i = 0; i < primes_len; ++i)
        is_fd[primes[i]] = 1;

    /* higher powers with exponent a power of two */
    for (size_t i = 0; i < primes_len; ++i) {
        seq_t val = (seq_t)primes[i] * (seq_t)primes[i];   /* p² */
        while (val <= (seq_t)MAX_LIMIT) {
            is_fd[(idx_t)val] = 1;
            seq_t next = val * val;   /* exponent doubles each step */
            if (next > (seq_t)MAX_LIMIT) break;
            val = next;
        }
    }
}

/* --- pre‑computation routine -------------------------------------------*/
static void init_sequence(void)
{
    if (cache_ready) return;   /* already built */
    cache_ready = true;

    build_primes();           /* primes up to MAX_LIMIT */
    mark_fd_numbers();        /* mark Fermi‑Dirac numbers */

    /* collect all a(n) in order */
    for (idx_t n = 5; n + 2 <= MAX_LIMIT; ++n) {
        if (is_fd[n] && is_fd[n + 2]) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)n;
        }
    }

    /* clean up temporary arrays */
    free(is_fd);
    free(is_composite);
    free(primes);
}

/* --- public function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, guard against erroneous callers */
    init_sequence();           /* ensure the table exists */

    if (n > cache_len)         /* caller requested beyond known range */
        return 0;

    return cache[n - 1];
}
