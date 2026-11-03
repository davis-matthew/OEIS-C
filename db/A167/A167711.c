/*
 * A167711  Numbers such that sum of digits is one more than a prime.
 * https://oeis.org/A167711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth integer k such that sum_of_digits(k) - 1 is prime
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 63 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_candidate = 1;   /* next integer to test */

    /* initialise cache */
    if (cache == NULL)
    {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
    }

    /* grow cache as needed */
    while (cache_len < (size_t)n)
    {
        /* compute digit sum of next_candidate */
        int sum_digits = 0;
        seq_t tmp = next_candidate;
        while (tmp > 0)
        {
            sum_digits += (int)(tmp % 10);
            tmp /= 10;
        }

        /* test condition */
        if (sum_digits > 1)
        {
            seq_t prime_candidate = (seq_t)(sum_digits - 1);
            if (is_prime_seq(prime_candidate))
            {
                if (cache_len == cache_cap)
                {
                    cache_cap *= 2;
                    cache = mem_resize(seq_t, cache, cache_cap);
                }
                cache[cache_len++] = next_candidate;
            }
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
