/*
 * A036963  Primes with digits (0,...,7) taken as base 8 and converted to base
 *          10.
 * https://oeis.org/A036963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A036963 – Primes with digits (0,…,7) taken as base 8 and converted to base 10
   The n‑th element of the sequence is the n‑th integer p (in ascending
   order) such that when p is written in base 8 and that octal string is
   interpreted as a decimal integer, the result is prime.

   Implementation uses lazy generation and caching.  The candidate integer
   is taken as a normal u64; from it we recover the base‑8 digits,
   reverse them to obtain the correct decimal value, and test primality
   with the supplied `isPrimeGeneric`.  Results are stored in a static
   dynamic array so that arbitrary permutations of calls are fast. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Return true if the octal representation of decimal number `n` interpreted
   as a decimal integer is prime.  The conversion reconstructs the digits
   in the correct order (most significant first). */
static bool is_octal_dec_prime(uint64_t n)
{
    /* Base-8 digits are obtained least‑significant first.  The
       maximum number of octal digits for a 64‑bit integer is 43. */
    uint8_t digits[64];
    size_t cnt = 0;
    while (n != 0) {
        digits[cnt++] = n & 7ULL;   /* n % 8 */
        n >>= 3;                    /* n /= 8 */
    }
    if (cnt == 0)          /* the number zero itself – not prime */
        return false;

    seq_t m = 0;
    for (size_t i = cnt; i-- > 0; ) {
        m = m * 10 + digits[i];
    }
    return isPrimeGeneric(m);
}

/* Cached sequence values. */
seq_t a(idx_t n)
{
    /* The cache grows lazily.  We keep track of the number of stored terms,
       the current capacity, and the last integer examined. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t last_checked = 0;

    /* Initialise cache on first use. */
    if (cache == NULL) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        ++last_checked;                               /* next candidate */
        if (is_octal_dec_prime(last_checked)) {
            if (cache_len == cache_cap) {             /* grow cache */
                cache_cap *= 2;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) abort();
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}
