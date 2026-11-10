/*
 * A065294  Values of k such that A065293(k) = 0.
 * https://oeis.org/A065294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A065293(k) = 0, i.e. no s in [0,k-1]
 *            with 2^s mod k = s (search, memoized)
 * Range:     u8 n <= 100, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 897 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A065294_zero(uint64_t k)
{
    uint64_t p = 1 % k;
    for (uint64_t s = 0; s < k; ++s)
    {
        if (p == s)
            return false;
        p = (p * 2) % k;
    }
    return true;
}

static uint64_t *A065294_cache = 0;
static uint64_t A065294_cnt = 0, A065294_cap = 0, A065294_next = 1;

seq_t a(idx_t n)
{
    while (A065294_cnt < (uint64_t)n)
    {
        uint64_t k = A065294_next++;
        if (A065294_zero(k))
        {
            if (A065294_cnt == A065294_cap)
            {
                A065294_cap = A065294_cap ? A065294_cap * 2 : 1024;
                A065294_cache = (uint64_t *)realloc(A065294_cache, A065294_cap * sizeof(uint64_t));
            }
            A065294_cache[A065294_cnt++] = k;
        }
    }
    return (seq_t)A065294_cache[n - 1];
}
