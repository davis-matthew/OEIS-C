/*
 * A018563  Divisors of 638.
 * https://oeis.org/A018563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A018563 – nth divisor of 638 (in ascending order). */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* for factor_u64 */

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------
       Precompute all divisors of 638 once, store them in a static
       array, and answer queries in O(1) time.
       ------------------------------------------------------------ */
    static bool initialized = false;
    static seq_t divisors[64];
    static size_t div_count = 0;

    if (!initialized)
    {
        /* Factor 638: 2 * 11 * 29 */
        uint64_t primes[16];
        uint64_t exps[16];
        size_t prime_cnt = 0;
        factor_u64((uint64_t)638, primes, exps, &prime_cnt);

        /* Generate divisors: start with 1, then for each prime add
           multiples of prime^k for k = 1..exps[i] */
        div_count = 1;
        divisors[0] = (seq_t)1;

        for (size_t i = 0; i < prime_cnt; ++i)
        {
            uint64_t p = primes[i];
            uint64_t exp = exps[i];
            /* Compute p^k sequentially */
            seq_t mul = 1;
            for (uint64_t k = 1; k <= exp; ++k)
            {
                mul *= (seq_t)p;           /* p^k */
                /* For every existing divisor, multiply by p^k */
                size_t old_cnt = div_count;
                for (size_t j = 0; j < old_cnt; ++j)
                {
                    divisors[div_count++] = divisors[j] * mul;
                }
            }
        }

        /* Sort divisors ascending – small count, simple insertion sort */
        for (size_t i = 1; i < div_count; ++i)
        {
            seq_t key = divisors[i];
            size_t j = i;
            while (j > 0 && divisors[j-1] > key)
            {
                divisors[j] = divisors[j-1];
                --j;
            }
            divisors[j] = key;
        }
        initialized = true;
    }

    if (n == 0 || n > div_count)
        return (seq_t)0;  /* undefined – return 0 */

    return divisors[n - 1];
}
