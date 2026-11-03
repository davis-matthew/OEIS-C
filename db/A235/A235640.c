/*
 * A235640  Primes p of the form n^2 + 1234567890 where 1234567890 is the
 *          first pandigital number with digits in order.
 * https://oeis.org/A235640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1956, u64 n <= 6694, u128 n <= 6694,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 6434 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Computes the nth prime of the form k^2 + 1234567890. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Static state: dynamically grow the cache as needed. */
    static seq_t *cache = NULL;   /* array of primes found so far */
    static size_t len = 0;        /* number of stored primes */
    static size_t cap = 0;        /* capacity of the array */
    static idx_t next_k = 1;      /* next k to test */

    static const seq_t BASE = (seq_t)1234567890u;

    /* If we already have the answer, return it. */
    if (n <= len)
        return cache[n-1];

    /* Find more primes until we have at least n of them. */
    while (len < n) {
        seq_t k = (seq_t)next_k;
        seq_t p = BASE + k * k;
        if (isPrimeGeneric(p))
            append_candidate(&cache, &len, &cap, p);
        ++next_k;
    }
    return cache[n-1];
}
