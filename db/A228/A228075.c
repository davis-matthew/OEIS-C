/*
 * A228075  Numbers n whose 10's complement is prime, i.e., 10^k-n, where k is
 *          the number of digits of n, is prime.
 * https://oeis.org/A228075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th positive integer n such that 10^k − n is prime, 
 * where k is the decimal digit count of n. 
 * The function incrementally computes the sequence, caching the values
 * so that any order of calls is handled efficiently. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* 1‑based cache: cache[0] = a(1), cache[1] = a(2), … */
    static seq_t *cache   = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t last_checked = 2;   /* next integer to test */

    if (n == 0) return 0;   /* undefined for index 0, return 0 */

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        idx_t cur = last_checked++;               /* candidate */
        int k = digits(cur);                       /* number of digits */
        seq_t pow10k = pow10(k);                   /* 10^k */
        seq_t complement = pow10k - (seq_t)cur;     /* 10^k − cur */
        if (is_prime_big(complement))
        {
            /* Grow the cache if needed */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();  /* memory exhausted */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur;  /* store the term */
        }
    }
    return cache[n - 1];
}
