/*
 * A173297  Numbers k such that exactly one of k^2 + k - 1 and k^2 + k + 1 is
 *          prime.
 * https://oeis.org/A173297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers k such that exactly one of k^2 + k - 1 and k^2 + k + 1
 *            is prime
 * Range:     u8 n <= 9, u16 n <= 407, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 8393 (2026-11-03)
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
    static seq_t *cache = NULL;      /* found k values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of values currently stored */
    static idx_t  search_k = 1;      /* next k candidate to test */

    /* Grow cache until it contains at least n entries */
    while (cache_len < (size_t)n) {
        /* Compute the two numbers to test for primality */
        seq_t m  = (seq_t)search_k * ((seq_t)search_k + 1); /* k*(k+1) */
        seq_t m_minus = m - 1;                              /* k^2 + k - 1 */
        seq_t m_plus  = m + 1;                              /* k^2 + k + 1 */

        bool p_minus = isPrimeGeneric(m_minus);
        bool p_plus  = isPrimeGeneric(m_plus);

        if (p_minus ^ p_plus) {            /* exactly one is prime */
            /* Allocate or grow cache if needed */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
                if (!new_cache) { /* allocation failed – stop gracefully */
                    return (seq_t)0;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)search_k;
        }
        ++search_k;                         /* next candidate k */
    }

    return cache[n - 1];                     /* 1‑based indexing */
}
