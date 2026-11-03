/*
 * A056561  Numbers n such that n^2 + n + 41 (known as Euler's polynomial) is
 *          prime.
 * https://oeis.org/A056561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth non‑negative integer m such that m^2 + m + 41 is
 *            prime. The sequence starts as
 *            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,
 *            i.e. indices at which Euler's polynomial yields a prime
 * Range:     u8 n <= 15, u16 n <= 530, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 8077 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    /* Static cache of computed sequence values.  */
    static seq_t *cache = NULL;      /* array of sequence values */
    static size_t  cache_cap = 0;    /* allocated capacity */
    static idx_t   filled = 0;       /* number of terms currently stored */
    static idx_t   next_m = 0;       /* next integer to examine */

    /* Ensure some capacity, growing by a factor of two when needed. */
    if (cache_cap == 0) {
        cache_cap = 16;                       /* initial arbitrary size */
        cache = mem_new(seq_t, cache_cap);
    }

    /* Generate terms until we have at least n of them. */
    while (filled < n) {
        /* Compute m^2 + m + 41 in seq_t arithmetic. */
        seq_t m    = (seq_t)next_m;
        seq_t poly = m * m + m + (seq_t)41;

        if (isPrimeGeneric(poly)) {
            /* Store the new term.  Grow the cache if full. */
            if (filled == cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[filled++] = m;
        }
        next_m++;
    }

    /* Return the desired term (indices in the function are 1‑based). */
    return cache[n - 1];
}
