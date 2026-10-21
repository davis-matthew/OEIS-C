/*
 * A065060  Primes p such that prime(p) - pi(p) is a prime.
 * https://oeis.org/A065060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   primes p such that prime(p) - pi(p) is a prime
 * Range:     u8 n <= 4, u16 n <= 106, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation precomputes all needed primes on demand.
   It keeps an internal list of valid sequence terms and expands the
   sieve limit geometrically until the requested term is found.
   The algorithm recomputes from scratch each time the sieve limit
   is increased, which keeps the code simple and fast enough for
   n up to at least 10 000. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"        /* bool is_prime_big(seq_t n); */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t, uint64_t **, size_t *); */

static uint64_t *g_primes      = NULL;    /* array of primes up to current limit */
static size_t   g_primes_cnt   = 0;        /* number of primes in g_primes */
static idx_t    g_limit        = 0;        /* current sieve limit */

static seq_t   *g_seq_cache    = NULL;    /* cached sequence terms */
static size_t   g_seq_len      = 0;        /* number of terms stored */

/* ------------------------------------------------------------------ */
/*  Allocate and build the prime table up to |limit|                  */
/* ------------------------------------------------------------------ */
static void rebuild_primes(idx_t limit)
{
    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;

    build_primes_upto(limit, &new_primes, &new_cnt);

    /* replace the old table */
    free(g_primes);
    g_primes     = new_primes;
    g_primes_cnt = new_cnt;
    g_limit      = limit;
}

/* ------------------------------------------------------------------ */
/*  Recompute the sequence up to the current prime table               */
/* ------------------------------------------------------------------ */
static void recompute_sequence(void)
{
    free(g_seq_cache);
    g_seq_cache = NULL;
    g_seq_len   = 0;

    /* a conservative allocation; will grow automatically */
    size_t cache_cap = 20000;
    g_seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!g_seq_cache) return;            /* allocation failure */

    for (size_t i = 0; i < g_primes_cnt; ++i) {
        uint64_t p = g_primes[i];

        /* prime(p) requires the (p)-th prime; check bounds */
        if (p == 0 || p - 1 >= g_primes_cnt) break;

        seq_t prime_of_p = (seq_t)g_primes[p - 1];   /* p‑th prime */
        seq_t pi_p       = (seq_t)(i + 1);           /* primes <= p */

        seq_t diff = prime_of_p - pi_p;
        if (is_prime_big(diff)) {
            if (g_seq_len >= cache_cap) {
                /* grow the cache */
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(g_seq_cache, cache_cap * sizeof(seq_t));
                if (!tmp) { free(g_seq_cache); g_seq_cache = NULL; return; }
                g_seq_cache = tmp;
            }
            g_seq_cache[g_seq_len++] = (seq_t)p;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Ensure that the sequence contains at least |n| terms.  If not,    */
/*  expand the sieve limit (doubling each time) and rebuild.          */
/* ------------------------------------------------------------------ */
static void ensure_sequence(idx_t n)
{
    /* initial limit */
    if (g_limit == 0) {
        g_limit = 2000000ULL;             /* start with 2 M */
        rebuild_primes(g_limit);
        recompute_sequence();
    }

    /* keep enlarging until enough terms are present */
    while (g_seq_len < n) {
        g_limit <<= 1;                    /* double the limit */
        rebuild_primes(g_limit);
        recompute_sequence();
    }
}

/* ------------------------------------------------------------------ */
/*  Public interface required by the judge                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    ensure_sequence(n);                   /* guarantees a(n) exists */

    return g_seq_cache[n - 1];
}
