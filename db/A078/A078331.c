/*
 * A078331  First prime such that p^2==1 (mod n).
 * https://oeis.org/A078331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least prime p such that p^2 ≡ 1 (mod n)
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS offset is 1: a(1) is defined, indices < 1 are undefined. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/mod_mul.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1, return 0 pattern */
        return (seq_t)0;

    /* Cache of computed values, indexed from 0 up to the current maximum n. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    size_t idx = (size_t)n;
    if (idx >= cache_cap) {
        size_t new_cap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* out‑of‑memory */
        /* Zero‑initialize new slots; old slots already contain values. */
        for (size_t i = cache_cap; i < new_cap; ++i)
            tmp[i] = 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    if (cache[idx] != 0)            /* already computed */
        return cache[idx];

    /* Special case n == 1: every prime satisfies p^2 ≡ 1 (mod 1). */
    if (n == 1) {
        cache[1] = (seq_t)2;
        return cache[1];
    }

    /* Search for the smallest prime p with p^2 ≡ 1 (mod n). */
    uint64_t p = 2;                 /* first prime */
    uint64_t mod = (uint64_t)n;
    while (1) {
        uint64_t m = p % mod;
        uint64_t sq = mod_mul(m, m, mod);  /* (m*m) mod mod  */
        if (sq == 1) {
            cache[idx] = (seq_t)p;
            return cache[idx];
        }
        p = next_prime(p);          /* go to the next prime */
    }
}
