/*
 * A139531  Numbers k such that 24*k + 17 is prime.
 * https://oeis.org/A139531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute the n-th integer k such that 24*k + 17 is prime.
   This function memoizes found k values so it works for any order
   of calls.  It uses isPrimeGeneric to test primality.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"

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

seq_t a(idx_t n)
{
    /* static cache of k values */
    static idx_t *cache = NULL;     /* array of found k */
    static size_t cache_len = 0;    /* number of entries in cache */
    static size_t cache_cap = 0;    /* allocated capacity of cache */
    static idx_t next_k = 0;        /* next k to test */

    /* grow cache if needed */
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        idx_t *tmp = (idx_t *)realloc(cache, new_cap * sizeof(idx_t));
        if (!tmp) abort();           /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate until we have at least n entries */
    while (cache_len < n) {
        seq_t candidate = 24 * (seq_t)next_k + 17;
        if (isPrimeGeneric(candidate)) {
            cache[cache_len++] = next_k;
        }
        next_k++;
    }

    return (seq_t)cache[n - 1];
}
