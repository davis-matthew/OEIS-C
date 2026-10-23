/*
 * A180554  Primes that become a different prime under the mapping 5 <=> 9.
 * https://oeis.org/A180554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 395, u32 n <= 396, u64 n <= 1595,
 *            u128 n <= 1595, big any n
 * Helpers:   is_prime_big, map5to9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A180554: primes that become a different prime under the mapping 5 <=> 9 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/is_prime_big.h"   /* declares bool is_prime_big(seq_t) */
#include "../utils/map5to9.h"

/* Return the n‑th term of A180554 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* safety: sequence is 1‑based */

    /* Cache of already found terms, static for successive calls. */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* how many terms have been cached */
    static size_t cap = 0;      /* current capacity of the cache array */
    static seq_t next_candidate = 2;  /* next integer to test for primality */

    /* Extend cache until it holds n terms. */
    while (cached < (size_t)n)
    {
        for (seq_t x = next_candidate; ; ++x)
        {
            if (is_prime_big(x))
            {
                seq_t y = map5to9(x);
                if (y != x && is_prime_big(y))
                {
                    /* Ensure capacity. */
                    if (cached >= cap)
                    {
                        size_t new_cap = cap ? cap << 1 : 64;
                        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
                        if (!tmp)
                            abort(); /* allocation failure */
                        if (cache)
                        {
                            memcpy(tmp, cache, cached * sizeof(seq_t));
                            free(cache);
                        }
                        cache = tmp;
                        cap = new_cap;
                    }

                    cache[cached++] = x;
                    next_candidate = x + 1;

                    /* If we have reached the required position, return it. */
                    if (cached == (size_t)n)
                        return x;

                    /* Stop the inner loop and continue searching for the next term. */
                    break;
                }
            }
        }
    }

    /* Return the requested term from the cache. */
    return cache[n - 1];
}
