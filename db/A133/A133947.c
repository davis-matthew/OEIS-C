/*
 * A133947  a(n) = the number of "non-isolated divisors" of n(n+1). A positive
 *          divisor, k, of n is non-isolated if (k-1) or (k+1) also divides n.
 * https://oeis.org/A133947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of non-isolated divisors of n(n+1)
 * Range:     u8 n <= 15, u16 n <= 594, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9393 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* Recursively enumerate all divisors of m and count the non-isolated ones.
   curr   – current divisor value (uint64_t)
   idx    – index in the list of prime factors
   primes – array of prime factors
   exps   – array of corresponding exponents
   pf_cnt – number of distinct prime factors
   m      – n*(n+1) as seq_t, used for divisibility tests
   cnt    – pointer to counter */
static void
count_non_isolated(uint64_t idx, uint64_t curr,
                   const uint64_t *primes, const uint64_t *exps,
                   size_t pf_cnt, seq_t m,
                   uint32_t *cnt)
{
    if (idx == pf_cnt) {
        if (curr == 1) {
            /* 1 is always non‑isolated, because 2 | n(n+1) 
               (product of two consecutive numbers is even). */
            (*cnt)++;
        } else {
            seq_t val = (seq_t)curr;
            seq_t t1 = m % (val - 1);
            seq_t t2 = m % (val + 1);
            if (t1 == 0 || t2 == 0)
                (*cnt)++;
        }
        return;
    }
    uint64_t p = primes[idx];
    uint64_t power = 1;
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        count_non_isolated(idx + 1, curr * power, primes, exps,
                           pf_cnt, m, cnt);
        power *= p;
    }
}

static seq_t *cache = NULL;   /* memoisation table */
static size_t cache_cap = 0;  /* current capacity of cache */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined, but safe */

    /* lazy cache expansion */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 2 : cache_cap;
        while (new_cap < need)
            new_cap <<= 1;
        cache = mem_resize(seq_t, cache, new_cap);
        for (size_t i = cache_cap; i < new_cap; ++i)
            cache[i] = 0;
        cache_cap = new_cap;
    }
    if (cache[n] != 0)
        return cache[n];

    /* compute m = n*(n+1) as seq_t to use % operator later */
    seq_t m = (seq_t)n * ((seq_t)n + 1);

    /* factor n and n+1 (they are coprime) */
    uint64_t primes_n[20], exps_n[20];
    uint64_t primes_np1[20], exps_np1[20];
    size_t cnt_n = 0, cnt_np1 = 0;

    factor_u64((uint64_t)n, primes_n, exps_n, &cnt_n);
    factor_u64((uint64_t)n + 1, primes_np1, exps_np1, &cnt_np1);

    /* merge the two factorizations into a single list */
    uint64_t primes[40], exps[40];
    size_t pf_cnt = 0;
    for (size_t i = 0; i < cnt_n; ++i) {
        primes[pf_cnt] = primes_n[i];
        exps[pf_cnt]   = exps_n[i];
        ++pf_cnt;
    }
    for (size_t i = 0; i < cnt_np1; ++i) {
        primes[pf_cnt] = primes_np1[i];
        exps[pf_cnt]   = exps_np1[i];
        ++pf_cnt;
    }

    /* enumerate all divisors and count the non-isolated ones */
    uint32_t cnt = 0;
    count_non_isolated(0, 1, primes, exps, pf_cnt, m, &cnt);

    cache[n] = (seq_t)cnt;
    return cache[n];
}
