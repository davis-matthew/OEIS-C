/*
 * A088738  Sum of semiprime divisors of n-th composite number.
 * https://oeis.org/A088738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of semiprime divisors of the n‑th composite number (n
 *            1‑based; a(1) corresponds to the first composite 4)
 * Range:     u8 n <= 201, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"   /* bool is_prime_small(uint64_t) */
#include "../utils/factor_u64.h"       /* void factor_u64(uint64_t, uint64_t *, uint64_t *, size_t *) */

seq_t a(idx_t n)
{
    /* ignore calls with n == 0; OEIS offset is 1 */
    if (n == 0)
        return 0;

    /* ----------- cache of composites --------------------------------- */
    static uint64_t *comps      = NULL;
    static size_t   comps_len   = 0;          /* how many composites stored */
    static size_t   comps_cap   = 0;          /* allocated capacity */
    static uint64_t next_candidate = 4;      /* first composite */

    /* grow the cache until it contains the first n composites */
    if (comps_len < n) {
        /* ensure capacity */
        if (comps_len < n && (size_t)n > comps_cap) {
            size_t new_cap = comps_cap > 0 ? comps_cap * 2 : 16;
            while (new_cap < n)
                new_cap *= 2;
            uint64_t *tmp = (uint64_t *)realloc(comps, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            comps   = tmp;
            comps_cap = new_cap;
        }
        /* add composites until we have n of them */
        while (comps_len < n) {
            if (!is_prime_small(next_candidate))
                comps[comps_len++] = next_candidate;
            next_candidate++;
        }
    }

    /* ------------------------------------------------------------------ */
    uint64_t m = comps[n - 1];   /* the n‑th composite number */

    /* factorize m (at most 16 prime factors for 64‑bit numbers) */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t   fact_cnt = 0;
    factor_u64(m, primes, exps, &fact_cnt);

    seq_t sum = 0;

    /* add square of each prime with exponent ≥ 2  (p^2 divides m) */
    for (size_t i = 0; i < fact_cnt; ++i) {
        if (exps[i] >= 2)
            sum += (seq_t)primes[i] * (seq_t)primes[i];
    }

    /* add product of each distinct pair of primes (p*q divides m) */
    for (size_t i = 0; i < fact_cnt; ++i) {
        for (size_t j = i + 1; j < fact_cnt; ++j) {
            sum += (seq_t)primes[i] * (seq_t)primes[j];
        }
    }

    return sum;
}
