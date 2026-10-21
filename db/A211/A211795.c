/*
 * A211795  Number of (w,x,y,z) with all terms in {1,...,n} and w*x < 2*y*z.
 * https://oeis.org/A211795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4‑tuples (w,x,y,z) with 1 ≤ w,x,y,z ≤ n and w*x
 *            < 2*y*z
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return 0;

    /* Compute n^2, which fits in idx_t (n ≤ 10000) */
    idx_t nn = n * n;                 /* nn = n² */
    size_t sz = (size_t)nn + 1;       /* size of frequency array */
    uint32_t *freq = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!freq)
        return 0;                     /* allocation failure → return 0 */

    /* Initialise frequency array to 0 */
    for (size_t i = 0; i < sz; ++i)
        freq[i] = 0;

    /* Count ordered pairs (y,z) with product = t */
    for (idx_t y = 1; y <= n; ++y)
    {
        for (idx_t z = 1; z <= n; ++z)
        {
            size_t prod = (size_t)(y * z);   /* product fits in 32 bits */
            freq[prod] += 1;
        }
    }

    /* Convert freq to a cumulative prefix sum:
       freq[t] now holds the number of pairs (y,z) with product <= t */
    for (size_t t = 1; t < sz; ++t)
        freq[t] += freq[t - 1];

    seq_t total_pairs = (seq_t)n * (seq_t)n;  /* n² as seq_t */
    seq_t sum = 0;

    /* For each (w,x) compute how many (y,z) satisfy w*x < 2*y*z */
    for (idx_t w = 1; w <= n; ++w)
    {
        for (idx_t x = 1; x <= n; ++x)
        {
            idx_t prod = w * x;                       /* w*x */
            idx_t thresh = prod / 2;                  /* floor((w*x)/2) */
            seq_t cnt = total_pairs - freq[thresh];   /* #pairs (y,z) with y*z > thresh */
            sum += cnt;
        }
    }

    free(freq);
    return sum;
}
