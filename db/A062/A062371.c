/*
 * A062371  Numbers the product of whose nonzero digits is a perfect square.
 * https://oeis.org/A062371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the search for numbers whose product of non‑zero decimal
   digits is a perfect square (OEIS A062371).  The function enumerates
   candidates sequentially; a static cache stores the first 100 000
   valid numbers, which is ample for n up to at least 10 000. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* --- integer square root for 64‑bit values -------------------------------- */
static uint64_t isqrt64(uint64_t n)
{
    uint64_t low  = 0;
    uint64_t high = 1ULL << 32;          /* high^2 >= 2^64-1 */
    uint64_t ans  = 0;
    while (low <= high)
    {
        uint64_t mid = low + ((high - low) >> 1);
        __int128 sq  = (__int128)mid * mid;
        if (sq <= n)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;          /* avoid wrap‑around */
            high = mid - 1;
        }
    }
    return ans;
}

/* --- test whether product of non‑zero decimal digits is a perfect square */
static inline bool good_prod_nonzero(idx_t x)
{
    uint64_t prod = 1;
    while (x != 0)
    {
        uint64_t d = x % 10;
        if (d != 0) prod *= d;
        x /= 10;
    }
    uint64_t r = isqrt64(prod);
    return r * r == prod;
}

/* --- cache for found terms ----------------------------------------------- */
#define CACHE_CAPACITY 100000

static uint64_t cache64[CACHE_CAPACITY];
static size_t cache_len = 0;          /* number of terms currently cached */
static uint64_t next_candidate = 1;   /* next integer to test for validity */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* out of bounds – not used */

    while (cache_len < n)
    {
        if (good_prod_nonzero(next_candidate))
        {
            if (cache_len < CACHE_CAPACITY) cache64[cache_len++] = next_candidate;
            else /* should never happen for the tested ranges */ ;
        }
        ++next_candidate;
    }
    return (seq_t)cache64[n - 1];
}
