/*
 * A176547  Numbers n such that 2*n^2 + 6*n + 1 is prime.
 * https://oeis.org/A176547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 30, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that 2*n^2 + 6*n + 1 is prime.
   a(1) = 3, a(2) = 6, a(3) = 15, … (offset 1)   */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

/* Return the n‑th member of the sequence (1‑based index).  */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t cached_cnt = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_checked = 0;   /* largest n already examined */

    if (n == 0)          /* undefined offset – return 0 */
        return (seq_t)0;

    /* already computed? */
    if ((idx_t)n <= cached_cnt)
        return cache[(size_t)n - 1];

    /* generate terms until we reach the desired index */
    idx_t t = last_checked;

    while (cached_cnt < (size_t)n)
    {
        ++t;

        /* compute p = 2*t^2 + 6*t + 1 in seq_t */
        seq_t val = 2 * (seq_t)t * (seq_t)t + 6 * (seq_t)t + 1;

        /* prime test */
        if (is_prime_big(val))
        {
            /* grow cache if necessary */
            ensure_capacity(cached_cnt + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
            cache[cached_cnt] = (seq_t)t;
            ++cached_cnt;
        }
    }

    last_checked = t;
    return cache[(size_t)n - 1];
}
