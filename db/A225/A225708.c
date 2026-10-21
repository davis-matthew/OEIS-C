/*
 * A225708  Composite squarefree numbers n such that p(i)-8 divides n+8, where
 *          p(i) are the prime factors of n.
 * https://oeis.org/A225708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   abs_prime_minus8, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A225708 (offset 1).
   Sequence: composite square‑free integers n such that for every prime
   factor p of n, p−8 divides n+8. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */
#include "../utils/abs_prime_minus8.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cache_cap  = 0;      /* current capacity */
    static size_t cache_len  = 0;      /* number of stored terms */
    static uint64_t last_checked = 1;  /* largest integer already examined */

    /* Ensure storage for at least n terms */
    if (cache_len < (size_t)n) {
        size_t need = n;
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap > 0)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate new terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;
        uint64_t m = last_checked;

        /* Factor m */
        uint64_t primes[32];
        uint64_t exps[32];
        size_t cnt = 0;
        factor_u64(m, primes, exps, &cnt);

        /* Skip primes or 1 */
        if (cnt <= 1) continue;

        /* Check square‑free and the divisibility condition */
        bool ok = true;
        seq_t plus8 = (seq_t)m + 8;
        for (size_t i = 0; i < cnt; ++i) {
            if (exps[i] > 1) { ok = false; break; }          /* not square‑free */
            unsigned long long diff = abs_prime_minus8(primes[i]);
            if (diff == 0) { ok = false; break; }            /* impossible prime 8 */
            if (plus8 % (seq_t)diff != 0) { ok = false; break; }
        }
        if (!ok) continue;

        /* Store term */
        cache[cache_len++] = (seq_t)m;
    }

    return cache[n - 1];
}
