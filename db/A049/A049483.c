/*
 * A049483  Primes p such that p + 2310 is also prime, where 2310 is the 5th
 *          primorial number A002110(5).
 * https://oeis.org/A049483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3025, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9304 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth prime p for which p + 2310 is also prime.
   It uses a cached list of previously found terms and expands it only
   enough to satisfy each request.  The search proceeds by iterating over
   consecutive primes `p` (using `next_prime`) and testing primality of
   `p + 2310` with `is_prime_big`.  The sequence is defined for n ≥ 1, so
   a(1) = 23, a(2) = 29, … . */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* --- static state ----------------------------------------------------- */
static seq_t  *cache      = NULL;   /* stored terms of the sequence   */
static size_t  cache_sz   = 0;      /* number of stored terms          */
static size_t  cache_cap  = 0;      /* allocated capacity              */
static uint64_t next_candidate = 2; /* next prime to examine           */
static bool     first_call     = true; /* flag for first initialization  */

/* --- helper ------------------------------------------------------------ */
static void fill_cache(idx_t n)
{
    /* Ensure the cache holds at least n terms.  We keep generating
       primes until we find the required number of terms. */
    while (cache_sz < n) {
        if (first_call) {
            /* First call: initialise the candidate to 2 */
            next_candidate = 2;
            first_call     = false;
        }

        uint64_t p = next_candidate;                /* current prime */
        /* Test whether p + 2310 is prime.  The sum fits into seq_t
           because for the range of n we consider it stays far below
           the limits of seq_t. */
        if (is_prime_big((seq_t)(p + 2310U))) {
            /* Grow the cache if needed */
            if (cache_sz == cache_cap) {
                cache_cap = cache_cap ? 2 * cache_cap : 64;
                cache = mem_resize(seq_t, cache, cache_cap);
                if (!cache) {
                    /* Allocation failure: abort the program. */
                    abort();
                }
            }
            cache[cache_sz++] = (seq_t)p;
        }

        /* Advance to the next prime candidate. */
        next_candidate = next_prime(p + 1);
    }
}

/* --- required public function ------------------------------------------ */
seq_t a(idx_t n)
{
    /* n is 1‑based; ensure we have at least n values */
    fill_cache(n);
    return cache[n - 1];
}
