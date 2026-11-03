/*
 * A050716  Inserting a digit '6' between all adjacent digits of k makes a
 *          prime.
 * https://oeis.org/A050716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 68, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Validated: standard, pass, 71 terms through n = 987 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer k with at least two decimal digits such that
 * inserting the digit 6 between all adjacent digits of k produces a prime.
 * Implementation uses a simple scan of successive integers and caches found
 * terms. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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
    /* static cache of already found terms */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;
    static idx_t cache_cap   = 0;
    static idx_t last_checked = 0;   /* largest integer examined so far */

    /* one-time allocation of cache array */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        ++last_checked;                 /* next candidate k */

        /* break candidate into decimal digits (least‑significant first) */
        idx_t tmp = last_checked;
        uint8_t digits[64];
        int dcnt = 0;
        while (tmp > 0) {
            digits[dcnt++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
        if (dcnt < 2)                /* skip single–digit numbers */
            continue;

        /* build the number obtained by inserting 6 between adjacent digits */
        seq_t inserted = digits[dcnt - 1];            /* most significant digit */
        for (int i = dcnt - 2; i >= 0; --i) {
            inserted = inserted * 10 + 6;           /* insert 6 */
            inserted = inserted * 10 + digits[i];   /* next original digit */
        }

        /* check primality of the inserted number */
        if (isPrimeGeneric(inserted)) {
            /* enlarge cache if necessary */
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_size++] = last_checked;     /* store valid k */
        }
    }

    return cache[n - 1];   /* sequence is 1‑based */
}
