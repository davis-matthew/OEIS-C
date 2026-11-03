/*
 * A167840  Six-times-isolated primes: primes p such that none of p+-2, p+-4,
 *          p+-6, p+-8, p+-10 nor p+-12 are prime.
 * https://oeis.org/A167840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 344, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 57 terms through n = 8886 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167840 (Six-times-isolated primes)
   a(n) = the nth prime p such that none of p±2, p±4, p±6, p±8, p±10
   nor p±12 are prime.  Offset 1. */

#include <stdlib.h>
#include <stdbool.h>
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

/* Find the n-th six-times-isolated prime.  The results are
   memoized in a static cache so that non‑monotonic calls are
   efficient. */
seq_t a(idx_t n)
{
    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t cur = 2;   /* next candidate to examine */

    if (n == 0) {
        /* Undefined for n < 1; return 0 to avoid UB */
        return (seq_t)0;
    }

    /* Expand cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        /* Search for the next prime starting from 'cur' */
        seq_t p = cur;
        while (!isPrimeGeneric(p)) {
            p += (seq_t)1;
        }
        cur = p + (seq_t)1;   /* set start for the next search */

        /* Check isolation: no p±d is prime for d = 2,4,6,8,10,12 */
        bool isolated = true;
        for (int d = 2; d <= 12; d += 2) {
            seq_t offset = (seq_t)d;
            seq_t ap = p + offset;
            if (isPrimeGeneric(ap)) {
                isolated = false;
                break;
            }
            if (p > offset) {
                seq_t am = p - offset;
                if (isPrimeGeneric(am)) {
                    isolated = false;
                    break;
                }
            }
        }
        if (!isolated) {
            continue;   /* not a member of the sequence */
        }

        /* Store the isolated prime */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) {
                /* Allocation failure – abort */
                exit(1);
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = p;
    }

    /* Return the requested term (1‑based index) */
    return cache[(size_t)n - 1];
}
