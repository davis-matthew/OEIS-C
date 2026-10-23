/*
 * A177214  Numbers k that are the products of two distinct primes such that
 *          2*k-1, 4*k-3, 8*k-7, 16*k-15 and 32*k-31 are also products of two
 *          distinct primes.
 * https://oeis.org/A177214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A177214: Numbers k that are the product of two distinct primes
   such that 2*k-1, 4*k-3, 8*k-7, 16*k-15 and 32*k-31 are also products of
   two distinct primes. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#if defined(__cplusplus)
using u128 = unsigned __int128;
#else
typedef unsigned __int128 u128;
#endif

// Prime limit used for the trial division factorisation.
#define PRIMELIMIT 2000000UL

/* ----------------------------------------------------------------------
 * Prime sieve: compute all primes up to PRIMELIMIT.  Run only once. */
static uint32_t *global_primes = NULL;
static size_t primes_cnt = 0;

static void init_primes(void)
{
    size_t limit = PRIMELIMIT;
    bool *is_comp = (bool*)calloc(limit + 1, sizeof(bool));
    global_primes = (uint32_t*)malloc((limit / 10) * sizeof(uint32_t));
    primes_cnt = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            global_primes[primes_cnt++] = (uint32_t)i;
            if (i <= limit / i) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    global_primes = (uint32_t*)realloc(global_primes, primes_cnt * sizeof(uint32_t));
    free(is_comp);
}

/* ----------------------------------------------------------------------
 * Test whether n is a product of exactly two distinct primes
 * (a square‑free semiprime). */
static bool is_semiprime_sqf(uint64_t n)
{
    if (n < 6) return false;          // 2*3=6 is the smallest semiprime

    uint64_t rem = n;
    int distinct = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = global_primes[i];
        if ((uint64_t)p * p > rem) break;
        if (rem % p == 0) {
            ++distinct;
            if (distinct > 2) return false;
            rem /= p;
            if (rem % p == 0) return false;   /* repeated factor -> not square‑free */
        }
    }

    if (rem == 1) {
        return distinct == 2;
    } else { /* rem > 1: last factor is a prime > sqrt(original) */
        return distinct == 1;
    }
}

/* ----------------------------------------------------------------------
 * Cache for sequence values.  Thread‑safe behaviour is not required. */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static void add_to_cache(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_size >= cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_size++] = val;
}

/* ----------------------------------------------------------------------
 * Main function requested by the build.  It returns a(n) for any n ≥ 1,
 * memoising previously found terms. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* out of range; undefined */

    /* Initialisation on first call. */
    static bool init_done = false;
    if (!init_done) {
        init_primes();
        init_done = true;
    }

    static uint64_t next_k = 2; /* start scanning from the smallest candidate */

    /* Generate terms until we have the n-th term. */
    while (cache_size < (size_t)n) {
        if (is_semiprime_sqf(next_k)) {
            bool ok = true;
            const int mult[5] = {2, 4, 8, 16, 32};
            const int sub[5]  = {1, 3, 7, 15, 31};

            for (int i = 0; i < 5 && ok; ++i) {
                u128 prod = ((u128)mult[i]) * (u128)next_k;
                u128 w128 = prod - (u128)sub[i];
                if (w128 > UINT64_MAX) {      /* cannot factor; skip candidate */
                    ok = false;
                    break;
                }
                if (!is_semiprime_sqf((uint64_t)w128))
                    ok = false;
            }
            if (ok) add_to_cache((seq_t)next_k);
        }
        ++next_k;
    }

    return cache[n - 1];
}
