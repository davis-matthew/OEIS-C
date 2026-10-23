/*
 * A156621  5-almost primes of the form p^2 - 2, where p is prime.
 * https://oeis.org/A156621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 22, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   ensure_capacity, factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = p^2–2 for the nth prime p such that p^2–2 has exactly
 * five prime factors (counted with multiplicity). */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* zero‑based index */
    if (n == 0) return (seq_t)0;

    /* static storage for results */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t next_p = 2;   /* next prime to test */

    /* grow cache until we have n entries */
    while (cache_len < n)
    {
        uint64_t p = next_p;

        /* value p^2 - 2 */
        seq_t val = (seq_t)p * (seq_t)p - (seq_t)2;
        uint64_t val64 = (uint64_t)val;  /* for factorization */

        /* factorization */
        uint64_t pr[32], ex[32];
        size_t cnt;
        factor_u64(val64, pr, ex, &cnt);

        /* compute total number of prime factors (with multiplicity) */
        uint64_t total = 0;
        for (size_t i = 0; i < cnt; ++i)
            total += ex[i];

        if (total == 5)
        {
            /* store if it is a 5‑almost prime */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = val;
        }

        /* advance to the next prime */
        next_p = next_prime(p + 1);
    }

    return cache[n - 1];
}
