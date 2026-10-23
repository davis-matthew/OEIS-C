/*
 * A172195  Prime numbers for which the absolute difference between the
 *          summation of its digits & the product of its digits is a prime.
 * https://oeis.org/A172195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = the n-th prime p such that |sum_of_digits(p) – product_of_digits(p)| is prime
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static uint32_t *all_primes = NULL;
    static size_t all_cap   = 0, all_cnt   = 0;

    static seq_t   *good       = NULL;
    static size_t   good_cap   = 0, good_cnt   = 0;

    static uint32_t next_candidate = 2;   /* start with the first prime */

    /* return cached value if already computed */
    if (n <= good_cnt)
        return good[n - 1];

    /* initialise arrays the first time */
    if (!all_primes)
    {
        all_cap = 1024;
        all_primes = (uint32_t *)malloc(all_cap * sizeof(uint32_t));
    }
    if (!good)
    {
        good_cap = 1024;
        good = (seq_t *)malloc(good_cap * sizeof(seq_t));
    }

    /* generate primes until we have enough good ones */
    while (good_cnt < n)
    {
        /* test primality of next_candidate */
        bool prime = true;
        for (size_t i = 0; i < all_cnt; ++i)
        {
            uint64_t d = all_primes[i];
            if (d * d > next_candidate)
                break;
            if (next_candidate % d == 0)
            {
                prime = false;
                break;
            }
        }

        if (prime)
        {
            /* store prime in all_primes list */
            if (all_cnt == all_cap)
            {
                all_cap <<= 1;
                all_primes = (uint32_t *)realloc(all_primes, all_cap * sizeof(uint32_t));
            }
            all_primes[all_cnt++] = next_candidate;

            /* compute sum, product, difference and check primality */
            seq_t sum  = digit_sum((seq_t)next_candidate);
            seq_t prod = digit_product((seq_t)next_candidate);
            seq_t diff = sum > prod ? sum - prod : prod - sum;

            if (diff >= (seq_t)2 && is_prime_big(diff))
            {
                /* store matching prime */
                if (good_cnt == good_cap)
                {
                    good_cap <<= 1;
                    good = (seq_t *)realloc(good, good_cap * sizeof(seq_t));
                }
                good[good_cnt++] = (seq_t)next_candidate;
            }
        }

        /* advance to next candidate
         * after 2 only odd numbers are needed */
        next_candidate = (next_candidate <= 2) ? 3 : next_candidate + 2;
    }

    return good[n - 1];
}
