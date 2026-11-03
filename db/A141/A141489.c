/*
 * A141489  Numbers k such that k^2 + k + 257 is prime.
 * https://oeis.org/A141489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 141, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 950 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer k such that k^2 + k + 257 is prime
   (OEIS A141489).  The function searches increasing k values
   and stores the found ones in a static cache so that any order
   of calls returns the correct term. */

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

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* unused; offset starts at 1 */

    /* static cache of the k values that satisfy the condition */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of elements currently stored   */
    static size_t cap   = 0;   /* current allocated capacity in elements*/
    static idx_t  next_k = 0;  /* next integer to test                */

    /* grow cache until it contains at least n elements */
    while (len < n) {
        seq_t k = (seq_t)next_k;               /* candidate integer       */
        seq_t val = k * k + k + (seq_t)257;    /* k^2 + k + 257           */

        if (isPrimeGeneric(val)) {             /* test primality          */
            if (len == cap) {                 /* enlarge storage as needed */
                size_t newcap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = k;                  /* store found k           */
        }
        ++next_k;                              /* try next integer        */
    }

    return cache[n - 1];                       /* return the nth term     */
}
