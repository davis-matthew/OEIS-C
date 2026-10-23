/*
 * A032685  Numbers k such that k surrounded by digit '9' is a prime.
 * https://oeis.org/A032685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 26, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, is_prime_big, mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032685: numbers k such that 9k9 is prime (search) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/mul_small.h"
#include "../utils/is_prime_big.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static seq_t next_k       = 0;   /* last tested k */

    while (cache_size < (size_t)n)
    {
        seq_t k = ++next_k;   /* candidate k, start at 1 */

        /* number of decimal digits of k */
        int d = digit_len(k);

        /* compute m = 9*10^(d+1) + k*10 + 9 */
        seq_t m = mul_small(pow10((idx_t)(d + 1)), 9U);
        m = m + mul_small(k, 10U);
        m = m + (seq_t)9;

        if (is_prime_big(m))
        {
            /* enlarge cache if needed */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = k;
        }
    }
    return cache[n - 1];
}
