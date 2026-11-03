/*
 * A056956  Numbers n such that 6n+1 and 6n+5 are both primes.
 * https://oeis.org/A056956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that 6k+1 and 6k+5 are
 *            both primes
 * Range:     u8 n <= 15, u16 n <= 923, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 75 terms through n = 4905 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incremental search with memoization and a primality test. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Offset is 1; no values requested for n == 0. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t found = 0;          /* number of terms stored in cache */
    static idx_t last_candidate = 1; /* next integer to test */

    /* Grow cache to accommodate n terms. */
    if (n > cap) {
        ensure_capacity((size_t)n, &cap, (void **)&cache, sizeof(*cache));
    }

    /* Generate new terms until we have at least n. */
    while (found < n) {
        seq_t p1 = (seq_t)6 * last_candidate + 1;
        seq_t p5 = p1 + 4; /* 6k+5 */

        if (isPrimeGeneric(p1) && isPrimeGeneric(p5)) {
            /* Store the candidate. */
            cache[found] = last_candidate;
            ++found;
        }
        ++last_candidate;
    }

    return cache[n - 1];
}
