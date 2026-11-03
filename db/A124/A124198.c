/*
 * A124198  Numbers k such that 21*k + 1 is prime.
 * https://oeis.org/A124198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth integer k such that 21*k + 1 is prime
 * Range:     u8 n <= 3, u16 n <= 568, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 899 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
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
    if (n == 0) return 0;               /* undefined for n < 1; offset is 1 */

    /* static cache of discovered terms */
    static seq_t *cache = NULL;          /* array of found k-values */
    static size_t cache_size = 0;        /* number of terms stored */
    static size_t cache_capacity = 0;    /* allocated array size */
    static idx_t next_candidate_k = 1;   /* next k to test */

    /* increase cache capacity if needed */
    if (cache_size < (size_t)n) {
        size_t new_cap = cache_capacity ? cache_capacity : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();               /* allocation failed */
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* generate further terms until we have at least n terms */
    while (cache_size < (size_t)n) {
        seq_t cand = (seq_t)21 * (seq_t)next_candidate_k + 1;
        if (isPrimeGeneric(cand)) {
            cache[cache_size++] = (seq_t)next_candidate_k;
        }
        next_candidate_k++;
    }

    return cache[n - 1];
}
