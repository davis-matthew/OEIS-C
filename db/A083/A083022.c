/*
 * A083022  Numbers n such that 4*n^2 - 3 is prime.
 * https://oeis.org/A083022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer m such that 4*m^2 – 3 is prime
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 975 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper for primality testing on seq_t values. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Cache of computed terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of terms stored */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Next integer to test for the property 4*m^2 – 3 prime. */
static idx_t next_candidate = 1;

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap <<= 1;   /* double until large enough */

    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();          /* memory allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* out of range, undefined by OEIS definition */

    /* Generate terms until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        idx_t m = next_candidate++;
        /* Compute 4*m^2 – 3 safely using 128‑bit arithmetic. */
        unsigned __int128 val128 = (unsigned __int128)4 * m * m - 3;
        seq_t val = (seq_t)val128;
        if (isPrimeGeneric(val)) {
            /* Store found integer m in the cache. */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)m;
        }
    }
    return cache[n - 1];
}
