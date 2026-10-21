/*
 * A081765  Numbers k such that k+2 divides 2^(k-1) - 1.
 * https://oeis.org/A081765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the k-th positive integer such that (k+2) divides 2^(k-1)
 *            – 1
 * Range:     u8 n <= 25, u16 n <= 1884, u32 n <= 2519, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_solution
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_solution.h"

static seq_t *cache = NULL;            /* stored sequence values */
static idx_t  cache_len = 0;           /* number of values stored */
static idx_t  cache_cap = 0;           /* allocated capacity */
static seq_t  next_candidate = 1;      /* next k to test in the search */

static void ensure_capacity(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    /* grow capacity: start at 1 and double until enough */
    if (cache_cap == 0)
        cache_cap = 1;
    while (cache_cap < needed)
        cache_cap <<= 1U;
    cache = (seq_t *)realloc(cache, (size_t)cache_cap * sizeof(seq_t));
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1, but guard for safety */
        return 0;

    if (n > cache_len)          /* generate further terms as needed */
    {
        ensure_capacity(n);
        while (cache_len < n)
        {
            seq_t k = next_candidate;
            if (is_solution(k))
            {
                cache[cache_len++] = k;
            }
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
