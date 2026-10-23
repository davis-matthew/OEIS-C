/*
 * A174800  Number whose product of digits is a square.
 * https://oeis.org/A174800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by scanning integers and selecting those whose digit product
   is a perfect square. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for unsigned long long. */
static unsigned long long int_sqrt_ull(unsigned long long x)
{
    unsigned long long lo = 0, hi = x;
    while (lo <= hi)
    {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned long long mid2 = mid * mid;
        if (mid2 == x)
            return mid;
        if (mid2 < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Returns true if the product of the decimal digits of v is a square.
   A zero digit makes the product zero, which is a perfect square. */
static bool digit_product_is_square(seq_t v)
{
    if (v == 0)
        return true;                 /* 0 has product 0 */

    unsigned long long prod = 1;
    seq_t temp = v;
    while (temp > 0)
    {
        unsigned long long d = (unsigned long long)(temp % 10);
        if (d == 0)
            return true;             /* product becomes 0 */
        prod *= d;
        temp /= 10;
    }

    unsigned long long s = int_sqrt_ull(prod);
    return s * s == prod;
}

/* The sequence itself, cached for efficiency. */
seq_t a(idx_t n)
{
    /* Static cache state. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t next_candidate = 1;

    /* Initialise cache with a(0) = 0. */
    if (cache_size == 0)
    {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failed, unlikely */
        cache[0] = 0;
        cache_size = 1;
        next_candidate = 1;
    }

    /* Return cached value if already known. */
    if (n < cache_size)
        return cache[(size_t)n];

    /* Generate further terms until we reach index n. */
    while (cache_size <= (size_t)n)
    {
        seq_t cand = next_candidate;
        ++next_candidate;

        if (digit_product_is_square(cand))
        {
            /* Grow cache if necessary. */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *new_buf = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                if (!new_buf)
                    return (seq_t)0;      /* realloc failed, unlikely */
                cache = new_buf;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;
        }
    }

    return cache[(size_t)n];
}
