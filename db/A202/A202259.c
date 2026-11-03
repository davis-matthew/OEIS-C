/*
 * A202259  Right-truncatable nonprimes: every prefix is nonprime number.
 * https://oeis.org/A202259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9846 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Purpose: Enumerate all nonnegative integers whose every decimal
   prefix is a nonprime (0 and 1 are considered nonprime).  The
   sequence is returned in ascending order; a(1) = 0, a(2) = 1, etc.
   The implementation caches previously computed terms so that
   subsequent calls are O(1) after the first computation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/*---------------------------------  helpers  ---------------------------------*/
static bool rt_nonprime(seq_t x)
{
    /* Return true iff every decimal prefix of x is nonprime. */
    while (true) {
        if (isPrimeGeneric(x))
            return false;          /* a prefix is prime → reject */
        if (x == (seq_t)0)
            break;                 /* 0 is nonprime and terminates */
        x /= (seq_t)10;            /* drop the rightmost digit */
    }
    return true;                  /* all prefixes nonprime */
}

/*---------------------------------  cache  ---------------------------------*/
static seq_t *cache   = NULL;   /* array of computed terms   */
static size_t cache_len  = 0;   /* number of terms stored    */
static size_t cache_cap  = 0;   /* allocated capacity        */
static seq_t next_candidate = (seq_t)0;   /* next integer to test */

/* Ensure the cache can hold at least new_cap elements. */
static void ensure_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        exit(1);                 /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/*------------------------------  main function  ------------------------------*/
seq_t a(idx_t n)
{
    /* n is at least 1 by specification; compute until we have n terms. */
    while (cache_len < n) {
        if (rt_nonprime(next_candidate)) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
        /* advance to next integer for the next iteration */
        next_candidate++;
    }
    return cache[n - 1];
}
