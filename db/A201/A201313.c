/*
 * A201313  Primes of the form n^2 - 10.
 * https://oeis.org/A201313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 19, u32 n <= 2202, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Validated: standard, pass, 50 terms through n = 5683 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes of the form n^2 - 10 (search) */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    /* Static cache of primes found of the form k^2-10 */
    static seq_t *cache  = NULL;
    static size_t   cap  = 0;
    static size_t   len  = 0;
    static idx_t    next = 9;          /* first k giving a prime: 9^2-10 = 71 */

    /* Largest representable value of seq_t */
    const seq_t max_val = (seq_t)-1;

    while (len < n) {
        idx_t  k   = next;
        next += 2;                     /* skip even k: (even)^2-10 is even >2 */

        idx_t  cand64 = k * k - 10;     /* candidate in 64‑bit math */

        /* Skip candidates that do not fit into seq_t */
        if (sizeof(seq_t) < sizeof(idx_t) && cand64 > (idx_t)max_val)
            continue;

        seq_t cand = (seq_t)cand64;

        /* a candidate ≤ 1 cannot be prime */
        if (cand <= 1)
            continue;

        if (isPrimeGeneric(cand)) {
            /* Grow cache if needed */
            if (len == cap) {
                size_t new_cap = cap ? (cap << 1) : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();          /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = cand;
        }
    }

    return cache[n - 1];   /* 1‑based indexing as per OEIS */
}
