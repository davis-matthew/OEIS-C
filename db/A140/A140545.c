/*
 * A140545  Primes of form 17n + 6.
 * https://oeis.org/A140545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) returns the n-th prime of the form 17*k + 6 (n≥1)
 * Range:     u8 n <= 3, u16 n <= 408, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9808 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_k = 1;  /* next k to test for 17*k+6 */

    if (n == 0)
        return (seq_t)0;  /* undefined for n<1 */

    while (cache_len < n) {
        seq_t candidate = (seq_t)17 * (seq_t)next_k + (seq_t)6;
        if (isPrimeGeneric(candidate)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (tmp == NULL) /* out of memory, return 0 as failure */
                    return (seq_t)0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = candidate;
        }
        ++next_k;
    }
    return cache[n - 1];
}
