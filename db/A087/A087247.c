/*
 * A087247  Squarefree deficient nonprime numbers.
 * https://oeis.org/A087247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8758 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A087247 (square‑free deficient non‑prime numbers).
   The function searches integers in increasing order, keeps those that are
   square‑free, have sigma(n) < 2*n (deficient), and are not prime.  It
   caches results so that any order of calls is supported. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Computes whether x is square‑free and its sum of divisors.  The
   sum is returned in *sigma_out.  The function returns true iff x is,
   square‑free.  All arithmetic is performed with seq_t, so the code
   works for both fixed‑width and arbitrary‑precision builds. */
static bool squarefree_and_sigma(uint64_t x, seq_t *sigma_out) {
    bool squarefree = true;
    seq_t s = 1;                 /* running product for sigma          */
    uint64_t t = x;

    /* factor of 2 */
    int cnt = 0;
    while ((t & 1ULL) == 0) {
        t >>= 1;
        cnt++;
    }
    if (cnt > 0) {
        if (cnt > 1)
            squarefree = false;
        seq_t pow2 = 1;
        for (int i = 0; i <= cnt; ++i)
            pow2 *= (seq_t)2;
        /* sigma part: (2^(cnt+1)-1)/(2-1) = 2^(cnt+1)-1          */
        s *= (pow2 - 1);
    }

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            int c = 0;
            while (t % p == 0) {
                t /= p;
                ++c;
            }
            if (c > 1)
                squarefree = false;
            seq_t powp = 1;
            for (int i = 0; i <= c; ++i)
                powp *= (seq_t)p;
            /* sigma part: (p^(c+1)-1)/(p-1)                         */
            seq_t part = (powp - 1) / ((seq_t)p - 1);
            s *= part;
        }
    }

    if (t > 1) {                 /* remaining odd prime factor         */
        s *= (seq_t)(t + 1);
    }

    *sigma_out = s;
    return squarefree;
}

/* Function to test primality is not required: we detect primes via sigma. */

/* Internal cache of computed terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of terms stored            */
static size_t cache_cap  = 0;   /* allocated capacity of the cache    */
static uint64_t max_checked = 0;/* the largest integer examined so far */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache(size_t need) {
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort();               /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public function as required by the interface. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;  /* offset is 1 */

    /* Extend cache until it contains the nth term. */
    if (n > cache_len) {
        for (uint64_t m = max_checked + 1; cache_len < (size_t)n; ++m) {
            max_checked = m;

            seq_t sigma;
            bool sqfree = squarefree_and_sigma(m, &sigma);

            if (!sqfree)                               /* skip non‑sf      */
                continue;

            /* Detect primes: sigma(n) == n + 1  */
            if (sigma == (seq_t)m + 1)
                continue;                              /* skip primes      */

            /* deficient test */
            seq_t two_m = (seq_t)m * 2;
            if (sigma < two_m) {
                ensure_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)m;
            }
        }
    }

    return cache[(size_t)n - 1];
}
