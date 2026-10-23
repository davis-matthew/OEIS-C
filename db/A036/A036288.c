/*
 * A036288  a(n) = 1 + integer log of n: if the prime factorization of n is n
 *          = Product (p_j^k_j) then a(n) = 1 + Sum (p_j * k_j) (cf. A001414).
 * https://oeis.org/A036288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + sum_{p^k || n} p * k
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A036288: a(n) = 1 + sum of prime factors times their exponents. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 */
    if (n == 1) return 1;

    /* Memoization cache: index is the integer n */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    size_t idx = (size_t)n;

    /* Ensure cache big enough for the requested index */
    if (idx >= cap) {
        size_t newcap = (cap > 0 ? cap * 2 : 16);
        if (newcap < idx + 1) newcap = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {
            /* Memory allocation failed – fall back to recomputing without cache */
            if (cache) free(cache);
            cache = NULL;
            cap = 0;
        } else {
            cache = tmp;
            /* Initialize new entries to 0 (uncomputed marker) */
            for (size_t i = cap; i < newcap; ++i) cache[i] = 0;
            cap = newcap;
        }
    }

    /* Return cached value if available */
    if (cache && cache[idx]) return cache[idx];

    /* Factor n using the provided utility */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute the sum of prime*exponent plus 1 */
    seq_t sum = 1;
    for (size_t i = 0; i < cnt; ++i) {
        sum += (seq_t)primes[i] * (seq_t)exps[i];
    }

    /* Store in cache if memory is available */
    if (cache) cache[idx] = sum;
    return sum;
}
