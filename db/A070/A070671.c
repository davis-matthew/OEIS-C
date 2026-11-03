/*
 * A070671  Smallest m in range 2..n-1 such that m^6 == 1 mod n, or 1 if no
 *          such number exists.
 * https://oeis.org/A070671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9279 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A070671: smallest m in 2..n‑1 such that m^6 ≡ 1 (mod n),
   or 1 if no such m exists.  The value is returned as a seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static inline unsigned long long powmod_small(unsigned long long base,
                                              unsigned int exp,
                                              unsigned long long mod)
{
    /* modular exponentiation for small moduli (n ≤ 10000 in typical
       tests).  Uses 128‑bit multiplication to avoid overflow. */
    unsigned long long result = 1 % mod;
    unsigned long long b = base % mod;
    while (exp) {
        if (exp & 1U)
            result = (unsigned long long)((unsigned __int128)result * b % mod);
        b = (unsigned long long)((unsigned __int128)b * b % mod);
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Simple memoised computation.  cache[0] unused; cache[i] holds a(i)
       once computed.  For n == 1 or 2 return 1; for n > 2 search m from 2
       to n-1 for m^6 ≡ 1 (mod n).  If none found, return 1. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n <= 2) {
        /* a(1) = 1, a(2) = 1 */
        return (seq_t)1;
    }

    /* Ensure cache can hold up to index n */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_size);
        if (!tmp) {
            /* allocation failed; fall back to computing without caching */
            cache = tmp;
            cache_size = new_size;
        } else {
            cache = tmp;
            for (idx_t i = cache_size; i < new_size; ++i)
                cache[i] = 0;          /* 0 → not yet computed */
            cache_size = new_size;
        }
    }

    if (cache[n] != 0)
        return cache[n];                 /* already computed */

    /* Compute a(n) */
    for (idx_t m = 2; m < n; ++m) {
        unsigned long long r = powmod_small((unsigned long long)m, 6U, (unsigned long long)n);
        if (r == 1U) {
            cache[n] = (seq_t)m;
            return cache[n];
        }
    }
    /* No such m → result is 1 */
    cache[n] = 1;
    return 1;
}
