/*
 * A201850  Primes of the form 7n^2 - 4.
 * https://oeis.org/A201850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime of the form 7·n²−4 (n ≥ 1), 1‑based
 *            indexing
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 1085, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 48 terms through n = 4841 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
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

/*  seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined; avoid UB if called with 0 */

    static seq_t *prime_cache = NULL;
    static size_t cache_size = 0;   /* number of cached primes */
    static size_t cache_cap  = 0;   /* allocated capacity of the cache */
    static idx_t next_k = 1;        /* next k to test for 7k²-4 */

    while (cache_size < (size_t)n)
    {
        /* Compute the candidate 7·k²−4 using a wider type to avoid
           overflow in seq_t before the final cast. */
        unsigned long long k = (unsigned long long)next_k;
        unsigned long long val_ul = 7ULL * k * k - 4ULL;

        seq_t candidate = (seq_t)val_ul;

        if (isPrimeGeneric(candidate))
        {
            /* Grow the cache if necessary. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
                if (!tmp) exit(1);        /* abort on OOM */
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cache_size++] = candidate;
        }
        ++next_k;
    }

    return prime_cache[(size_t)(n - 1)];  /* adjust for 1‑based index */
}
