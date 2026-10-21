/*
 * A211809  Number of 4-tuples (w,x,y,z) with all terms in {1,...,n} and w*x
 *          >= 2*y*z.
 * https://oeis.org/A211809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (w,x,y,z) ∈ [1,n]^4 : w*x ≥ 2*y*z }|
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for product frequencies */
static uint16_t *freq_cache = NULL;
static idx_t cache_n = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Recompute cache if necessary */
    if (n > cache_n)
    {
        size_t size = (size_t)n * (size_t)n + 1;          /* allocate n² + 1 cells */
        free(freq_cache);
        freq_cache = (uint16_t *)calloc(size, sizeof(uint16_t));
        if (!freq_cache)    /* allocation failed */
            return 0;

        /* Fill frequency table: freq[p] = #{(w,x) : w*x = p} */
        idx_t limit = n;
        for (idx_t w = 1; w <= limit; ++w)
        {
            size_t wv = (size_t)w;
            for (idx_t x = 1; x <= limit; ++x)
                ++freq_cache[wv * (size_t)x];
        }
        cache_n = n;
    }

    /* Count the desired 4‑tuples */
    size_t maxP = (size_t)n * (size_t)n;          /* maximum product w*x */
    seq_t  sum = 0;
    uint64_t cum_boundary = 0;                    /* cumulative count for products ≤ current boundary */
    size_t prev_boundary = 0;                     /* the previous boundary value */

    for (size_t p = 1; p <= maxP; ++p)
    {
        /* Move the boundary to p/2, updating the cumulative sum */
        while (prev_boundary < p / 2)
        {
            ++prev_boundary;
            cum_boundary += freq_cache[prev_boundary];
        }

        /* freq_cache[p] contributes with all (y,z) pairs whose product ≤ p/2 */
        sum += (seq_t)freq_cache[p] * (seq_t)cum_boundary;
    }

    return sum;
}
