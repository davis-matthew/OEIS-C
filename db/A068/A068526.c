/*
 * A068526  Numbers n such that prime(n+1)-prime(n) = tau(n) = A000005(n).
 * https://oeis.org/A068526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 5367, u32 n <= 9740, u64 n <= 9740,
 *            u128 n <= 9740, big any n
 * Helpers:   build_primes_upto, build_spf, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A068526
    a(n) = the n‑th positive integer m such that
           prime(m+1) – prime(m) = τ(m)
    (τ(m) = number of positive divisors of m)          */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

/*-------------------------------------------------------------------------*/
/*  Internal state (static helpers and data)                              */
/*-------------------------------------------------------------------------*/
static uint64_t *primes      = NULL;   /* primes[0] = 2, primes[1] = 3, …            */
static size_t   prime_count   = 0;     /* number of primes stored                   */
static uint64_t *spf          = NULL;   /* spf[n] = smallest prime factor of n      */
static size_t   spf_limit     = 0;     /* numbers up to spf_limit are covered       */

static seq_t   *cache        = NULL;   /* table of sequence members                */
static size_t   cache_len    = 0;     /* number of filled entries in cache         */
static size_t   cache_cap    = 0;     /* current capacity of cache                */
static uint64_t max_checked   = 0;    /* largest m that has already been examined */

static bool     initialized  = false; /* true after init() has run                */

/*-------------------------------------------------------------------------*/
/*  Small helpers                                                        */
/*-------------------------------------------------------------------------*/
/* Compute τ(n) using the pre‑computed smallest‑prime‑factor array       */
static seq_t tau_of (idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    while (m > 1) {
        idx_t p = spf[m];
        idx_t e = 0;
        do {
            m /= p;
            ++e;
        } while (m % p == 0);
        result = result * ((seq_t)(e + 1));
    }
    return result;
}

/* Ensure that cache contains at least 'target' sequence entries.         */
static void ensure_seq (idx_t target)
{
    if (!initialized) return;         /* init() must have been called earlier */

    while (cache_len < target) {
        ++max_checked;                /* examine next integer m                */
        uint64_t m = max_checked;     /* 1‑based index of the integer          */

        /* stop if we ran out of pre‑computed primes or spf data          */
        if (m >= prime_count || m > spf_limit) return;

        seq_t tau   = tau_of(m);
        seq_t diff  = (seq_t)(primes[m] - primes[m - 1]);  /* prime(m+1)-prime(m) */

        if (tau == diff) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)m;
        }
    }
}

/* Initialise prime and divisor‑factor tables and the cache               */
static void init (void)
{
    if (initialized) return;
    initialized = true;

    /*---------------------------------------------------------------------*
     *  Choose a generous limit that comfortably contains the first        *
     *  10 000 terms of this sequence.  For that purpose 3 000 000 is      *
     *  sufficient: it yields about 216 000 primes and an SPF array that   *
     *  needs ~24 MB.  The constants can be tuned if necessary.           *
     *---------------------------------------------------------------------*/
    const idx_t LIMIT = 3000000ULL;

    build_primes_upto(LIMIT, &primes, &prime_count);
    build_spf(LIMIT, &spf);
    spf_limit = LIMIT;

    cache_cap = 8;
    cache = seq_alloc(cache_cap);
    cache_len = 0;
    max_checked = 0;
}

/*-------------------------------------------------------------------------*/
/*  Public entry point                                                   */
/*-------------------------------------------------------------------------*/
seq_t a (idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for 0, return 0 as safe fallback */

    if (!initialized) init();         /* first call initialises shared data       */
    ensure_seq(n);                    /* make sure cache contains n entries      */
    return cache[n - 1];              /* 1‑based indexing of the sequence element */
}
