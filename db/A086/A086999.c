/*
 * A086999  Periodic part of decimal expansion of 1/p for those primes having
 *          a periodic part of even length.
 * https://oeis.org/A086999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of OEIS A086999: periodic part of the decimal expansion
   of 1/p for those primes p whose multiplicative order of 10 modulo p
   is even.  For each qualifying prime the period is  
      r = (10^m − 1)/p            with  m the order.
   If p > 9 the period begins with a leading zero; the returned integer
   is r·10 so that the leading zero is removed.  The sequence is
   generated lazily and memoised so that any call order is allowed.
*/
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_len = 0;
    static uint64_t last_checked_prime = 1;   /* last prime examined */

    /* for n < 1 behaviour is irrelevant */
    if (n == 0) return 0;

    /* generate terms until we reach the requested index */
    while (cache_len < (size_t)n)
    {
        uint64_t p;
        for (p = last_checked_prime + 1; ; ++p)
        {
            /* skip non‑primes and the trivial factors of 10 */
            if (p == 2 || p == 5) continue;
            if (!is_prime((idx_t)p)) continue;

            /* multiplicative order of 10 modulo p */
            uint64_t phi = p - 1;
            uint64_t k   = phi;
            uint64_t temp = phi;

            /* trial division on phi to get prime factors */
            for (uint64_t f = 2; f * f <= temp; ++f)
            {
                if (temp % f == 0)
                {
                    while (temp % f == 0) temp /= f;
                    while (k % f == 0)
                    {
                        uint64_t cand = k / f;
                        if (mod_pow((seq_t)10, cand, (seq_t)p) == 1)
                            k = cand;
                        else
                            break;
                    }
                }
            }
            if (temp > 1)
            {
                uint64_t f = temp;
                while (k % f == 0)
                {
                    uint64_t cand = k / f;
                    if (mod_pow((seq_t)10, cand, (seq_t)p) == 1)
                        k = cand;
                    else
                        break;
                }
            }

            /* keep only even period lengths */
            if ((k & 1ULL) == 0)
            {
                last_checked_prime = p;

                /* grow the cache if necessary */
                if (cache_cap < cache_len + 1)
                {
                    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
#if defined(__cplusplus)
                    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
#else
                    seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
                    if (!tmp) abort();          /* memory allocation failed */
                    cache = tmp;
                    cache_cap = new_cap;
                }

                /* compute the period integer r = (10^k − 1)/p          */
                seq_t powm = big_pow((seq_t)10, (seq_t)k);
                seq_t r = (powm - (seq_t)1) / (seq_t)p;

                /* strip the common leading zero for p > 9            */
                if (p > 9) r *= (seq_t)10;

                /* store the value */
                cache[cache_len++] = r;
                break;    /* go on to next required term */
            }
            /* otherwise continue searching for next prime */
        }
    }

    return cache[(size_t)(n - 1)];
}
