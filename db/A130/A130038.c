/*
 * A130038  Even numbers n such that n-7 is prime, but neither n-3 nor n-5 is
 *          prime.
 * https://oeis.org/A130038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130038: the n‑th even number such that (n‑7) is prime but
   (n‑3) and (n‑5) are not.  The function lazily builds the sequence
   and memoises the found terms. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;
    static size_t cache_sz = 0;
    static size_t cache_cap = 0;
    static seq_t last_val = 0;

    if (n == 0) return 0;            /* undefined for n < 1 */

    while (cache_sz < n)
    {
        seq_t cand = (cache_sz == 0) ? 30 : last_val + 2;  /* next even */

        for (;;)
        {
            if (is_prime_big(cand - 7) &&
                !is_prime_big(cand - 3) &&
                !is_prime_big(cand - 5))
            {
                /* store the term */
                if (cache_sz == cache_cap)
                {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (cache_cap == 0)
                                 ? seq_alloc(new_cap)
                                 : seq_realloc(cache, cache_cap, new_cap);
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_sz++] = cand;
                last_val = cand;
                break;
            }
            cand += 2;   /* next even candidate */
        }
    }
    return cache[n - 1];
}
