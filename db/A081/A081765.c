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
 * Validated: standard, pass, 71 terms through n = 9867 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* test whether a given k satisfies (k+2) | (2^(k-1) – 1) */
static bool is_solution(seq_t k)
{
    seq_t m = k + 2;                    /* modulus */
    seq_t result = 1 % m;
    seq_t base   = 2 % m;
    idx_t exp    = (idx_t)(k - 1);      /* exponent fits in idx_t for the intended n-range */

    while (exp > 0)
    {
        if (exp & 1U)
            result = (result * base) % m;
        base = (base * base) % m;
        exp >>= 1U;
    }
    return result == 1;
}

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
    cache = mem_resize(seq_t, cache, (size_t)cache_cap);
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
