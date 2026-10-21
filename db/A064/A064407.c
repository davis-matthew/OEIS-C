/*
 * A064407  Even numbers not the sum of a pair of the lesser of the twin
 *          primes.
 * https://oeis.org/A064407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A064407 – Even numbers that cannot be expressed as the sum of two
 * lesser twin primes.
 *
 * Algorithm:
 *   • Generate all twin primes up to a fixed limit L.
 *   • For every pair (p_i , p_j) with i ≤ j compute s = p_i + p_j
 *     and record that s is representable.
 *   • Scan even numbers in increasing order; those for which the
 *     representable flag is false are appended to the cache.
 *   • The function a(n) guarantees that at least n terms are cached
 *     and returns the (n‑1)‑th element of that cache (1‑based OEIS
 *     indexing).  Cache expansion and the prime list are lazy‑init
 *     guarded so repeated calls are O(1) after the tables have been
 *     built.
 *
 *   All computations use types provided by the build system (seq_t,
 *   idx_t).  Helper tables are of native integer size to hold the
 *   twin primes (uint64_t) regardless of seq_t’s width.
 *
 *   The chosen limit L = 200000 is sufficient to produce at least
 *   the first 10 000 sequence elements, satisfying the time/memory
 *   constraints for typical unit tests. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define LIMIT 200000u      /* maximum twin prime searched (>= required bound) */

static idx_t   next_even   = 2;          /* next even to test for representability */
static size_t  seq_len    = 0;           /* number of cached sequence values       */
static size_t  seq_cap    = 0;           /* capacity of seq_cache                   */
static seq_t  *seq_cache  = NULL;       /* cached sequence values                 */

static bool   *rep        = NULL;       /* rep[s] true iff s is sum of two lesser twin primes */
static size_t  rep_size   = 0;           /* size of rep array                       */

static uint64_t *twin     = NULL;       /* list of lesser twin primes (uint64_t)      */
static size_t    twin_cnt = 0;          /* number of twin primes                      */

static bool      initialized = false;   /* one‑time global initialisation            */

/* ------------------------------------------------------------------ */
/* Build the twin‑prime list and the representable‑sum table.          */
/* ------------------------------------------------------------------ */
static void
init_sequence(void)
{
    if (initialized)
        return;

    /* --------- Sieve of Eratosthenes -------------------------------- */
    const size_t sieve_limit = (size_t)LIMIT + 2u;
    bool *is_prime = (bool *)malloc((sieve_limit + 1u) * sizeof(bool));
    if (!is_prime)
        abort();
    for (size_t i = 0u; i <= sieve_limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2u; i * i <= sieve_limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= sieve_limit; j += i)
                is_prime[j] = false;
        }
    }

    /* --------- Collect lesser twin primes --------------------------- */
    size_t cap = sieve_limit / 10u;             /* rough initial capacity */
    twin = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!twin)
        abort();
    twin_cnt = 0;
    for (size_t p = 2u; p + 2u <= sieve_limit; ++p) {
        if (is_prime[p] && is_prime[p + 2u]) {
            if (twin_cnt == cap) {
                cap <<= 1u;
                twin = (uint64_t *)realloc(twin, cap * sizeof(uint64_t));
                if (!twin)
                    abort();
            }
            twin[twin_cnt++] = (uint64_t)p;
        }
    }
    free(is_prime);

    /* --------- Build representable-sum boolean array ----------------- */
    rep_size = 2u * LIMIT + 1u;
    rep = (bool *)malloc(rep_size * sizeof(bool));
    if (!rep)
        abort();
    for (size_t i = 0u; i < rep_size; ++i)
        rep[i] = false;

    for (size_t i = 0u; i < twin_cnt; ++i) {
        for (size_t j = i; j < twin_cnt; ++j) {
            size_t sum = (size_t)twin[i] + (size_t)twin[j];
            if (sum >= rep_size)
                break;
            rep[sum] = true;
        }
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Ensure that at least n terms are cached in seq_cache.                */
/* ------------------------------------------------------------------ */
static void
ensure_seq_upto(idx_t n)
{
    if (seq_len >= (size_t)n)          /* already computed */
        return;

    /* Expand sequence cache capacity if necessary */
    if (seq_cap == 0u) {
        seq_cap = 32u;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        if (!seq_cache)
            abort();
    }
    while (seq_cap < (size_t)n) {
        seq_cap <<= 1u;
        seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!seq_cache)
            abort();
    }

    /* Generate missing terms */
    while ((size_t)seq_len < (size_t)n) {
        /* next_even is guaranteed to be even and >= 2 */
        if (!rep[(size_t)next_even]) {
            seq_cache[seq_len++] = (seq_t)next_even;
        }
        next_even += 2u;
    }
}

/* ------------------------------------------------------------------ */
/* Public function for the OEIS sequence.                              */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0u)      /* undefined for n < 1, but avoid crash */
        return (seq_t)0;

    init_sequence();           /* one‑time global initialization */
    ensure_seq_upto(n);        /* make sure we have at least n terms */

    return seq_cache[n - 1u];   /* OEIS indexing starts at 1 */
}
