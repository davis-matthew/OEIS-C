/*
 * A139219  Primes of the form 41+(n+n^2)/2=41+A000217(n).
 * https://oeis.org/A139219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 889 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A139219  –  Primes of the form 41 + n*(n+1)/2   (n = 0,1,2,…).
    a(n) = the n‑th prime among numbers of the form 41 + T(k), where
    T(k) = k*(k+1)/2 is the k‑th triangular number (offset 1).
*/

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

seq_t a(idx_t n)
{
    /* static cache for the primes found so far */
    static seq_t *cache    = NULL;   /* array of primes */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of primes cached */

    static idx_t  last_m     = 0;    /* last triangular index tested */
    static size_t primes_cnt = 0;    /* number of primes found so far */

    /* invalid index – return 0 */
    if (n == 0) return 0;

    /* return cached value if already computed */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* initialise cache */
    if (!cache) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
    }

    /* search until we have found the n‑th prime of the required form */
    while (primes_cnt < (size_t)n) {
        seq_t m   = (seq_t)last_m;           /* current triangular index */
        seq_t tri = m * (m + 1) / 2;         /* T(m) = m*(m+1)/2 */
        seq_t val = 41 + tri;                /* candidate prime */

        if (isPrimeGeneric(val)) {
            if (primes_cnt == cache_cap) {   /* grow cache if necessary */
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[primes_cnt] = val;         /* store prime */
            primes_cnt++;
        }
        last_m++;                            /* next triangular index */
    }

    cache_len = primes_cnt;                  /* update length */
    return cache[n - 1];
}
