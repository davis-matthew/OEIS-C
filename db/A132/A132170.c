/*
 * A132170  Primes of the form (3pq - q - 6)/(3p + 1), where p and q are
 *          prime.
 * https://oeis.org/A132170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A132170 ------------------------------------------------------------
    Sequence: Primes of the form (3pq - q - 6)/(3p + 1), where p and q are primes.
    Offset: 1
    Computes the n‑th prime that can be represented as
        a = (3*p*q - q - 6)/(3*p + 1)
   for some primes p, q.  The algorithm scans successive prime candidates a
   and tests whether a representation exists using primes p up to an
   explicit bound.
   -------------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

/* Utility routines supplied by the build system */
#include "../utils/is_prime_small.h"   /* bool is_prime_small(uint64_t n) */
#include "../utils/is_prime_big.h"     /* bool is_prime_big(seq_t n)   */

static seq_t *cache = NULL;           /* cached terms of the sequence           */
static size_t cache_size = 0;         /* number of terms cached                  */
static size_t cache_cap = 0;          /* allocated capacity for the cache        */

static uint64_t *prime_list = NULL;   /* list of all primes seen so far          */
static size_t prime_count = 0;        /* number of primes stored                 */
static size_t prime_cap = 0;          /* allocated capacity for prime list       */
static uint64_t last_prime_gen = 1;   /* largest prime generated so far          */

static idx_t a_prime_index = 0;       /* next index in prime_list to use as a   */

/* ------------------------------------------------------------------------ */
/* dynamic array helpers                                                    */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

static void ensure_prime_capacity(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap = new_cap;
}

/* ------------------------------------------------------------------------ */
/* prime generation: adds the next prime to prime_list and returns it       */
static uint64_t next_prime(void)
{
    uint64_t cand = last_prime_gen + 1;
    while (!is_prime_small(cand)) cand++;
    ensure_prime_capacity(prime_count + 1);
    prime_list[prime_count++] = cand;
    last_prime_gen = cand;
    return cand;
}

/* ------------------------------------------------------------------------ */
/* extend prime_list so that it contains all primes ≤ limit                 */
static void ensure_prime_up_to(uint64_t limit)
{
    while (last_prime_gen < limit) {
        next_prime();
    }
}

/* ------------------------------------------------------------------------ */
/* insert a value into cache if it is not already present, keeping cache sorted */
static void cache_insert_unique(seq_t val)
{
    /* binary search */
    size_t lo = 0, hi = cache_size;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (cache[mid] < val)
            lo = mid + 1;
        else if (cache[mid] > val)
            hi = mid;
        else
            return; /* already present */
    }

    /* insert at position lo */
    ensure_cache_capacity(cache_size + 1);
    memmove(cache + lo + 1, cache + lo, (cache_size - lo) * sizeof(seq_t));
    cache[lo] = val;
    cache_size++;
}

/* ------------------------------------------------------------------------ */
/* main sequence function                                                   */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of range, not used */

    /* generate terms until we have at least n entries */
    while (cache_size < (size_t)n) {
        /* obtain next prime candidate for a */
        if (a_prime_index >= prime_count)
            next_prime();           /* ensure list is long enough */
        uint64_t a_val_uint = prime_list[a_prime_index++];
        seq_t a_val = (seq_t)a_val_uint;

        /* bound on p: p ≤ (2a + 7)/3  (derived from q ≥ a+1) */
        uint64_t upper_p = ( ( ( (__uint128_t)2 * (uint64_t)a_val ) + 7 ) / 3 );

        /* ensure prime list contains all required p values */
        ensure_prime_up_to(upper_p);

        /* test each prime p up to the bound */
        bool found = false;
        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint64_t p = prime_list[pi];
            if (p > upper_p) break;

            /* compute q = (a*(3p+1)+6)/(3p-1) */
            __uint128_t num_q = ( (__uint128_t)a_val ) * (3 * p + 1) + 6;
            uint64_t den_q = 3 * p - 1;
            if (num_q % den_q != 0) continue;

            seq_t q_val = (seq_t)(num_q / den_q);
            if (q_val <= a_val) continue;          /* should not happen with bound */
            if (!is_prime_big(q_val)) continue;     /* q must be prime */

            found = true;
            break;
        }

        if (found) {
            cache_insert_unique(a_val);            /* store the term */
        }
    }

    return cache[(size_t)n - 1];
}
