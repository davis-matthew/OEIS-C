/*
 * A124253  Numbers k such that 1 + k^3 + k^5 is prime.
 * https://oeis.org/A124253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 21, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124253: a(n) = the n‑th integer k such that 1 + k³ + k⁵ is prime. */

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

/* a(n) returns the n‑th positive integer k (n ≥ 1) for which 1 + k³ + k⁵ is prime. */
seq_t a(idx_t n)
{
    /* Static state retained between calls. */
    static idx_t next_k   = 1;          /* smallest k to test next */
    static seq_t *list   = NULL;        /* dynamic array of found ks   */
    static size_t  sz     = 0;          /* number of ks stored         */
    static size_t  cap    = 0;          /* current capacity of list    */

    if (n == 0)              /* out of range, harmless */
        return 0;

    /* Grow the list until it has at least n elements. */
    while (sz < (size_t)n) {
        seq_t k  = (seq_t)next_k;
        seq_t k2 = k * k;          /* k² */
        seq_t k3 = k2 * k;         /* k³ */
        seq_t k5 = k3 * k2;        /* k⁵ */
        seq_t val = 1 + k3 + k5;   /* 1 + k³ + k⁵ */

        if (isPrimeGeneric(val)) {
            /* Ensure capacity. */
            if (sz == cap) {
                size_t new_cap = cap ? cap * 2 : 8;
                seq_t *tmp = mem_resize(seq_t, list, new_cap);
                if (!tmp) abort();
                list = tmp;
                cap  = new_cap;
            }
            list[sz++] = k;        /* store the qualifying k */
        }
        ++next_k;                  /* advance to next candidate */
    }

    return list[n - 1];
}
