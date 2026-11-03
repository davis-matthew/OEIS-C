/*
 * A124109  Numbers whose abundance (A033880) or deficiency (A033879) is a
 *          semiprime number.
 * https://oeis.org/A124109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 7831, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 9778 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A124109  (OEIS)  Numbers whose abundance or deficiency is a semiprime.
 * The value a(n) is the n‑th positive integer k ≥ 1 such that
 *    |σ(k) – 2·k|
 * is a semiprime.  The sequence starts 5, 7, 11, 12, 14, 15, 21,...
 *
 * This implementation scans the integers in increasing order, computes
 * σ(k) on the fly with the provided factor_u64 utility, detects whether
 * the absolute difference is a semiprime, and caches found terms.
 * Calls to a(n) are answered immediately from the cache and the scan
 * continues only when a larger n is requested.  The cache is allocated
 * once and grown as necessary.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* sum of divisors of n (uint64_t) */
static uint64_t sigma_of_u64(uint64_t n)
{
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    uint64_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        unsigned e = (unsigned)exps[i];
        uint64_t term = 1;
        uint64_t power = 1;
        for (unsigned j = 0; j < e; ++j) {
            power *= p;          /* p^(j+1) */
            term  += power;
        }
        result *= term;
    }
    return result;
}

/* return true if x is a semiprime (product of exactly two primes, counting multiplicity) */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4)                /* 4 = 2×2 is the smallest semiprime */
        return false;

    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);

    unsigned tot = 0;
    for (size_t i = 0; i < cnt; ++i)
        tot += (unsigned)exps[i];

    return tot == 2;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* undefined for n = 0 */

    /* one‑time allocation of dynamic cache */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t next_candidate = 1;    /* integer to test next */

    if (cache == NULL) {
        cap   = 1024;
        cache = mem_new(seq_t, cap);
    }

    /* generate terms until we have at least n of them */
    while (len < n) {
        uint64_t cand = (uint64_t)next_candidate;

        uint64_t sigma   = sigma_of_u64(cand);
        uint64_t two_cand = 2 * cand;
        uint64_t diff    = (sigma > two_cand) ? (sigma - two_cand)
                                               : (two_cand - sigma);

        if (is_semiprime_uint64(diff)) {
            if (len == cap) {
                cap *= 2;
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                if (tmp == NULL) {      /* allocation failed – abort */
                    mem_free(cache);
                    cache = NULL;
                    return (seq_t)0;
                }
                cache = tmp;
            }
            cache[len++] = (seq_t)cand;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
