/*
 * A226971  Numbers k such that the sum of digits of k^7 is equal to k.
 * https://oeis.org/A226971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th integer k such that sum of decimal digits of k^7 equals k
   (search) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/digit_sum.h"

/* maximum k to test – chosen so that k > 9*digits(k^7) can never hold
   for any reasonable seq_t (above 2000 the gap is far larger) */
static const idx_t MAX_K = 2000;

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1; behaviour for n<1 is irrelevant */

    /* static cache of found values and bookkeeping */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of elements currently stored */
    static size_t cache_cap = 0;   /* allocated capacity of cache */
    static idx_t next_k = 0;       /* next candidate k to test */

    /* grow cache until we have at least n elements or run out of k */
    while (cache_len < (size_t)n) {
        if (next_k > MAX_K)
            break;                /* no further candidates to check */

        seq_t k = (seq_t)next_k;              /* candidate number */
        seq_t p = big_pow(k, (seq_t)7);      /* compute k^7  */
        seq_t ds = digit_sum(p);             /* sum of its digits */

        if (ds == k) {                       /* property holds */
            /* ensure capacity of cache */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                        new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure – return 0 as sentinel */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;         /* store found value */
        }
        ++next_k;
    }

    /* if n exceeds the number of found entries, return 0 */
    if (cache_len < (size_t)n)
        return 0;

    return cache[n - 1];   /* sequence is 1‑based */
}
