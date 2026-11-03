/*
 * A173153  Values of n such that either {0,1}*n^2+n+41 is prime.
 * https://oeis.org/A173153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th positive integer m such that exactly one of m+41 or
 *            m^2+m+41 is prime
 * Range:     u8 n <= 11, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-03)
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

/* Cached sequence values. */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* number of terms stored */
static size_t cache_cap   = 0;      /* current allocation capacity */

/* Ensure the cache has room for at least `need` elements. */
static void grow_cache(size_t need)
{
    if (cache_cap >= need)
        return;
    if (cache_cap == 0)
        cache_cap = 16;
    while (cache_cap < need)
        cache_cap <<= 1;
    cache = mem_resize(seq_t, cache, cache_cap);
}

/* Return the n-th term of the sequence.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but guard against underflow */

    /* Go on generating terms until we have at least n of them. */
    static idx_t next_m = 1;   /* candidate integer to test */

    while (cache_size < n)
    {
        bool ok = false;

        /* test m + 41   (0 * m^2 part) */
        seq_t s1 = (seq_t)next_m + (seq_t)41;
        /* test m^2 + m + 41   (1 * m^2 part) */
        seq_t s2 = (seq_t)next_m * (seq_t)next_m + (seq_t)next_m + (seq_t)41;

        /* Exactly one of them must be prime. */
        ok = isPrimeGeneric(s1) ^ isPrimeGeneric(s2);

        if (ok)
        {
            grow_cache(cache_size + 1);
            cache[cache_size] = (seq_t)next_m;
            cache_size++;
        }
        next_m++;
    }

    return cache[n - 1];
}
