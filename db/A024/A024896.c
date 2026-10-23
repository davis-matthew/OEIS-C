/*
 * A024896  Numbers k such that 5*k - 2 is prime.
 * https://oeis.org/A024896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = the n-th integer k such that 5*k - 2 is prime (search) */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset 1; behaviour for n<1 is undefined */

    /* Static cache to allow out-of-order queries */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_k = 0;          /* last integer k that was tested */

    /* Generate terms until the cache contains at least n. */
    while (cache_len < n)
    {
        seq_t k = last_k + 1;         /* next candidate k */
        seq_t p = 5 * k - 2;          /* test number */

        if (is_prime_big(p))
        {
            if (cache_len >= cache_cap)
            {
                if (cache_cap == 0)
                    cache_cap = 64;
                else
                    cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp)
                    abort();          /* allocation failure – terminate */
                cache = tmp;
            }
            cache[cache_len++] = k;    /* store k as the next sequence value */
        }

        last_k = k;                    /* advance to next integer */
    }

    return cache[n - 1];               /* sequences are 1‑based */
}
