/*
 * A201108  Primes of the form 2n^3+3.
 * https://oeis.org/A201108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(2*(n-1)^3+3)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 145, u64 n <= 2700,
 *            u128 n <= 2700, big any n
 * Validated: standard, pass, 46 terms through n = 2557 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the nth prime of the form 2k^3+3.  An internal cache of
 *  found primes is kept so that the function works correctly for
 *  calls in any order.
 */

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

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /*  Cache of primes already found.  */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /*  Last 'k' value that was tested (starting with k = 0).  */
    static idx_t k  = 0;

    if (n < 1) return 0;      /*  Undefined for index below 1.  */

    /*  Generate primes until we have at least n of them.  */
    while (cache_len < n) {
        seq_t kseq = (seq_t)k;
        seq_t val  = kseq * kseq * kseq;   /*  k^3  */
        val = val * ((seq_t)2) + (seq_t)3; /*  2*k^3 + 3  */

        if (isPrimeGeneric(val)) {
            grow_seq((seq_t **)&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = val;
        }
        ++k;
    }

    return cache[n - 1];
}
