/*
 * A138566  Numbers n whose digits satisfy the following Diophantine equation:
 *          P = (X1* ... *Xr)/(X1+ ... +Xr), where P = prime number, Xi =
 *          digits of n.
 * https://oeis.org/A138566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth positive integer whose digits satisfy
   (digit_product)/(digit_sum) = prime.
   This implementation generates terms on demand and caches them
   for efficient retrieval in any order of calls. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* static cache for discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static uint64_t next_num = 1;  /* next number to test */

    /* generate terms until we have at least n of them */
    while (cache_sz < n)
    {
        uint64_t val = next_num++;
        seq_t sum = 0;
        seq_t prod = 1;

        /* compute product of digits and sum of digits */
        uint64_t tmp = val;
        while (tmp > 0)
        {
            uint64_t d = tmp % 10;
            tmp /= 10;
            sum += (seq_t)d;
            prod *= (seq_t)d;
            if (d == 0) {   /* product will be 0, skip early */
                prod = 0;
                break;
            }
        }

        /* check the Diophantine condition: prod = prime * sum */
        if (sum > 0 && prod % sum == 0)
        {
            seq_t prime = prod / sum;
            if (is_prime_big(prime))
            {
                /* store the original number into cache */
                append_candidate(&cache, &cache_sz, &cache_cap, (seq_t)val);
            }
        }
    }
    return cache[n - 1];
}
