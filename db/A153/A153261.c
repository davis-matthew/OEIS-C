/*
 * A153261  Primes p such that 3*p-2 is not prime.
 * https://oeis.org/A153261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th prime p such that 3*p-2 is NOT prime.
   The function incrementsally searches primes, tests 3*p-2 for
   compositeness, and memoises all found terms. */

/* Use the utilities supplied by the build */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>          /* for uint64_t if needed */
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

/* ------------------------------------------------------------------ */
/* External types supplied by the build:                            */
/*   idx_t  – unsigned 64‑bit integer (index type)                   */
/*   seq_t  – integer type (8, 16, 32, 64‑bit or arbitrary precision) */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Guard for non‑positive indices – the OEIS offset is 1. */
    if (n == 0) {
        return 0;
    }

    /* ------------------------------------------------------------------ */
    /* Memoised cache of terms already computed.                           */
    /* ------------------------------------------------------------------ */
    static seq_t *cache      = NULL;      /* array of valid primes   */
    static size_t cache_cnt  = 0;          /* number of stored terms */
    static size_t cache_cap  = 0;          /* capacity of the array  */

    /* ------------------------------------------------------------------ */
    /* Candidate prime generator                                         */
    /* ------------------------------------------------------------------ */
    static seq_t cand = 2;                 /* next number to test      */
    static bool first = true;              /* initialise after 2       */

    /* ------------------------------------------------------------------ */
    /* Ensure we have at least n terms in the cache.                     */
    /* ------------------------------------------------------------------ */
    while (cache_cnt < n)
    {
        seq_t p = cand;

        /* Advance candidate for next iteration   */
        if (first) {
            first = false;                  /* 2 has been processed   */
            cand = 3;                       /* next candidate 3       */
        } else {
            cand += 2;                      /* skip evens             */
        }

        /* Skip if p is not prime.               */
        if (!is_prime_big(p))
            continue;

        /* Compute 3*p-2 and test its primality. */
        seq_t t = 3 * p - 2;
        if (!is_prime_big(t))
        {
            /* Grow cache if needed.             */
            if (cache_cnt >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) {                 /* allocation failed     */
                    exit(EXIT_FAILURE);
                }
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_cnt++] = p;
        }
    }

    return cache[n - 1];
}
