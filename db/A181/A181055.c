/*
 * A181055  Numbers n such that sum_{k=1..n} (-1)^(n-k) *bigomega(k) is prime.
 * https://oeis.org/A181055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that S(k) = Σ_{i=1..k} (-1)^{k-i}
 *            * bigomega(i) is a prime > 1
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-03)
 * Helpers:   is_prime_i64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts with a(1)=4, a(2)=6, a(3)=8, … (OEIS A181055).  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_i64.h"
#include "../utils/mem.h"

/* Small helper: compute ω(k) – the total number of prime factors of k */
static unsigned long long bigomega_local(idx_t k)
{
    unsigned long long cnt = 0;
    if (k < 2) return 0;
    while ((k & 1ULL) == 0) { cnt++; k >>= 1; }
    for (idx_t p = 3; (unsigned long long)p * p <= k; p += 2) {
        while (k % p == 0) { cnt++; k /= p; }
    }
    if (k > 1) cnt++;            /* remaining prime factor */
    return cnt;
}

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* main sequence routine: returns a(n) as seq_t */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1 */

    /* Static state: cache of qualifying k values */
    static seq_t *cache     = NULL;  /* array of found a(i) values */
    static size_t cache_cap  = 0;     /* allocated capacity */
    static size_t cache_len  = 0;     /* number of elements stored */
    static idx_t  processed   = 0;    /* largest k already examined */
    static long long S_prev   = 0;    /* S(processed) */

    /* Grow cache as needed when retrieving further terms */
    while (cache_len < n) {
        processed++;  /* next candidate k */

        unsigned long long w = bigomega_local(processed); /* ω(k) */
        long long S_curr = (long long)w - S_prev;          /* S(k) = w − S_prev */

        if (S_curr > 1 && is_prime_i64(S_curr)) {
            /* Need room in cache */
            if (cache_len + 1 > cache_cap) {
                size_t newcap = (cache_cap == 0) ? 16 : cache_cap << 1;
                while (newcap <= cache_len + 1) newcap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                if (!tmp) { exit(EXIT_FAILURE); }
                cache = tmp;
                cache_cap = newcap;
            }
            cache[cache_len++] = (seq_t)processed;
        }

        S_prev = S_curr;  /* prepare for next k */
    }

    return cache[n - 1];
}
