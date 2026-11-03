/*
 * A142532  Primes congruent to 2 mod 53.
 * https://oeis.org/A142532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p ≡ 2 (mod 53)
 * Range:     u8 n <= 1, u16 n <= 131, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 670 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* cache of found primes */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t count  = 0;
    static idx_t k      = 0;   /* candidate index: p = 53*k + 2 */

    /* find primes until we have at least n of them */
    while (count < n)
    {
        idx_t cand = (53 * k) + 2;

        if (is_prime(cand))
        {
            /* grow the cache if necessary */
            if (cache == NULL)
            {
                cap = 1;
                cache = mem_new(seq_t, cap);
            }
            else if ((size_t)count >= cap)
            {
                cap <<= 1;   /* double capacity */
                cache = mem_resize(seq_t, cache, cap);
            }

            cache[count++] = (seq_t)cand;
        }
        ++k;
    }

    return cache[n - 1];
}
