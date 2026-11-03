/*
 * A201710  Primes of the form 10n^2 + 3.
 * https://oeis.org/A201710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 10 n^2 + 3 (OEIS A201710)
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 2269, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 51 terms through n = 4901 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next_k = 0;

    while (len < n) {
        seq_t k = (seq_t)next_k;
        seq_t candidate = ((seq_t)10 * k * k) + (seq_t)3;

        if (isPrimeGeneric(candidate)) {
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;              /* realloc failed: return 0 */
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = candidate;
        }
        ++next_k;
    }

    return cache[n - 1];
}
