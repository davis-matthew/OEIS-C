/*
 * A141791  Primes of the form c(n)-n, where c(n)=n-th composite.
 * https://oeis.org/A141791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime numbers of the form c(n)-n, where c(n) is the n‑th
 *            composite number
 * Range:     u8 n <= 319, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 90 terms through n = 984 (2026-11-03)
 * Helpers:   is_prime, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime.h"
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
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* state for generating terms lazily */
    static idx_t cursor   = 4; /* next integer to inspect (first composite) */
    static idx_t comp_idx = 0; /* count of composites seen */

    /* ensure enough capacity */
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        cache = mem_resize(seq_t, cache, cache_cap);
        if (!cache) return (seq_t)0; /* allocation failure */
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* advance cursor to the next composite */
        while (is_prime(cursor)) ++cursor;
        ++comp_idx; /* cursor now points to the comp_idx‑th composite */

        /* diff = c(k) - k */
        seq_t diff = (seq_t)cursor - (seq_t)comp_idx;

        /* store if diff is prime */
        if (isPrimeGeneric(diff)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                cache = mem_resize(seq_t, cache, cache_cap);
                if (!cache) return (seq_t)0;
            }
            cache[cache_len++] = diff;
        }
        ++cursor; /* move to next integer for next iteration */
    }

    return cache[n - 1];
}
