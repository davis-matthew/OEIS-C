/*
 * A080821  Primes in the accumulated tally of steps in the x+1 problem:
 *          Repeat until 1 is reached: if x is even divide by 2, otherwise add
 *          1.
 * https://oeis.org/A080821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth prime that appears as a cumulative sum of step
   counts for the x+1 problem; computed by iterating the problem
   and selecting primes. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static bool is_prime_seq(seq_t n)
{
    if (n < 2)                     return false;
    if (n == 2)                     return true;
    if ((n & 1) == 0)               return false;

    seq_t sq = floor_sqrt(n);
    idx_t lim = (idx_t)sq;   /* floor sqrt, limit for trial division */

    for (idx_t p = 3; p <= lim; p += 2)
    {
        if (n % (seq_t)p == 0)
            return false;
    }
    return true;
}

/* static state for sequence generation */
static idx_t next_start   = 1;   /* next starting value to process   */
static seq_t cumulative   = 0;   /* cumulative sum of step counts so far  */

seq_t a(idx_t n)
{
    static seq_t *prime_cache = NULL;
    static size_t cache_size = 0, cache_cap = 0;

    while (cache_size < (size_t)n)
    {
        /* compute number of steps for start = next_start */
        idx_t steps = 0;
        idx_t x = next_start;

        while (x > (idx_t)1)
        {
            if ((x & (idx_t)1) == 0)
                x >>= 1;
            else
                x += (idx_t)1;
            ++steps;
        }

        cumulative += (seq_t)steps;

        if (is_prime_seq(cumulative))
        {
            if (cache_cap == 0)
            {
                cache_cap = 16;
                prime_cache = mem_new(seq_t, cache_cap);
            }
            else if (cache_size == cache_cap)
            {
                cache_cap <<= 1;
                prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
            }
            prime_cache[cache_size++] = cumulative;
        }

        ++next_start;
    }

    return prime_cache[(size_t)n - 1];
}
