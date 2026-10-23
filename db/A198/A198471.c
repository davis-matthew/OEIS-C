/*
 * A198471  Abundant numbers that are smaller than the sum of their deficient
 *          divisors.
 * https://oeis.org/A198471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 6129, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, sigma_piece_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_piece_u64.h"

/* OEIS A198471:   a(n) = the n-th abundant number that is smaller than the
 * sum of its deficient divisors.
 * This implementation incrementally searches over all integers,
 * testing abundance and the deficient–divisor‑sum condition.
 */

/* Recursively generate all divisors of a number from its prime factorisation.
 * `pr` and `ex` store the prime bases and exponents.
 * `idx` is the current prime index, `cnt` the total number of primes.
 * `cur_div`  and `cur_sigma` are the divisor and its σ value accumulated so far.
 * All operations are performed with uint64_t to avoid overflow of seq_t.
 * `sum` is the running total of deficient divisors. */
static void gen_divisors(const uint64_t *pr, const uint64_t *ex,
                         size_t idx, size_t cnt,
                         uint64_t cur_div, uint64_t cur_sigma,
                         uint64_t *sum)
{
    if (idx == cnt) {
        uint64_t two_div = cur_div << 1;
        if (cur_sigma < two_div) {
            *sum += cur_div;
        }
        return;
    }

    uint64_t p = pr[idx];
    uint64_t emax = ex[idx];

    uint64_t pow_piece = 1;
    uint64_t sigma_piece = 1;
    for (uint64_t e = 0; e <= emax; ++e) {
        uint64_t div_next  = cur_div * pow_piece;
        uint64_t sigma_next = cur_sigma * sigma_piece;
        gen_divisors(pr, ex, idx + 1, cnt, div_next, sigma_next, sum);

        if (e < emax) {
            pow_piece   *= p;
            sigma_piece = sigma_piece * p + 1;
        }
    }
}

/* Main sequence function.  Uses static caching to reuse past results. */
seq_t a(idx_t n)
{
    /* Cache for previously found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 1;

    /* Grow the cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t m = next_candidate++;

        /* Prime factorisation of m. */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64(m, primes, exps, &cnt);

        /* Compute σ(m) as product of σ(p^e). */
        uint64_t sigma_m = 1;
        for (size_t i = 0; i < cnt; ++i) {
            sigma_m *= sigma_piece_u64(primes[i], exps[i]);
        }

        /* Skip if m is not abundant. */
        if (sigma_m <= (m << 1))
            continue;

        /* Sum deficient divisors of m. */
        uint64_t sum_deficient = 0;
        gen_divisors(primes, exps, 0, cnt, 1, 1, &sum_deficient);

        if (sum_deficient <= m)
            continue; /* m is not smaller than that sum */

        /* Append m to the cache. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len++] = (seq_t)m;
    }

    return cache[n - 1];
}
