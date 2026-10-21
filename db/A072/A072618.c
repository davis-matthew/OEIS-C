/*
 * A072618  Numbers n for which the prime circle problem has a simple
 *          solution: the arrangement of numbers 1 through 2n around a circle
 *          is such that the sum of each pair of adjacent numbers is prime and
 *          the odd and even numbers are in order in opposite directions.
 * https://oeis.org/A072618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) returns the nth integer m for which a simple solution to
 *            the * prime circle problem exists (OEIS A072618). The function
 *            searches * integers m starting at 1, keeps a running list of the
 *            ones that satisfy * the conditions (a),(b),(c) given in the OEIS
 *            entry, and returns the * element with inde...
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* constants for the prime sieve */
#define MAX_SIEVE 5000000UL             /* covers 4*m up to ≈1.5×10^6 for m up to ~200 000 */
#define TWIN_SIZE ((MAX_SIEVE-3)/2 + 5) /* number of possible k with 2*k+3 <= MAX_SIEVE */

/* global tables: 1=prime, 0=non‑prime ; twin[k]=1 if (2*k+1,2*k+3) is a twin pair */
static uint8_t is_prime[MAX_SIEVE + 1];
static uint8_t is_twin[TWIN_SIZE];

/* flag and data for memoizing the sequence */
static bool sieve_ready   = false;
static bool cache_ready   = false;
static idx_t  last_m      = 0;      /* last m inspected */
static idx_t  found_cnt   = 0;      /* number of valid terms found so far */
static seq_t *found_vals  = NULL;   /* array holding found terms */
static size_t cache_cap   = 0;      /* capacity of found_vals */

/* ------------------------------------------------------------------ */
/* initialise global tables on first use                              */
/* ------------------------------------------------------------------ */
static void init_sieve(void)
{
    if (sieve_ready) return;
    sieve_ready = true;

    /* simple Eratosthenes sieve up to MAX_SIEVE */
    for (idx_t i = 0; i <= MAX_SIEVE; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (idx_t p = 2; p * p <= MAX_SIEVE; ++p)
        if (is_prime[p])
            for (idx_t j = p * p; j <= MAX_SIEVE; j += p)
                is_prime[j] = 0;

    /* pre‑compute twin‑offset table */
    idx_t limit_k = (MAX_SIEVE - 3) / 2;
    for (idx_t k = 1; k <= limit_k; ++k)
        is_twin[k] = (is_prime[2 * k + 1] && is_prime[2 * k + 3]) ? 1 : 0;
}

/* ------------------------------------------------------------------ */
/* test whether a given m satisfies at least one of conditions (a),(b),(c) */
/* ------------------------------------------------------------------ */
static bool has_simple_solution(idx_t m)
{
    /* condition (a): 2m+1 and 2m+3 are prime */
    if (2 * m + 3 <= MAX_SIEVE &&
        is_prime[2 * m + 1] && is_prime[2 * m + 3])
        return true;

    /* condition (c): 2m-1, 2m+1 and 4m-1 are prime */
    if (4 * m - 1 <= MAX_SIEVE &&
        is_prime[2 * m - 1] && is_prime[2 * m + 1] && is_prime[4 * m - 1])
        return true;

    /* condition (b): find k with 1 ≤ k ≤ m-2 such that
     * both {2k+1,2k+3} and {2(k+m)+1,2(k+m)+3} are twin primes */
    if (m >= 3) {
        idx_t max_k = m - 2;
        for (idx_t k = 1; k <= max_k; ++k) {
            /* k+m must not exceed precomputed twin table size */
            if (k + m < TWIN_SIZE && is_twin[k] && is_twin[k + m])
                return true;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* maintain a cache of discovered terms                               */
/* ------------------------------------------------------------------ */
static void ensure_cache(void)
{
    if (cache_ready) return;
    cache_ready = true;
    cache_cap   = 64;      /* initial capacity */
    found_vals  = (seq_t *)malloc(cache_cap * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */
/* main exported function: a(idx_t n)                                 */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* initialise resources on first call */
    init_sieve();
    ensure_cache();

    /* extend the cache until it contains the n-th term */
    while (found_cnt < n) {
        ++last_m;
        if (has_simple_solution(last_m)) {
            /* store the new term */
            ++found_cnt;
            if (found_cnt > cache_cap) {
                cache_cap *= 2;
                found_vals = (seq_t *)realloc(found_vals, cache_cap * sizeof(seq_t));
            }
            found_vals[found_cnt - 1] = (seq_t)last_m;
        }
    }
    return found_vals[n - 1];
}
