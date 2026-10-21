/*
 * A212021  Number of (w,x,y,z) with all terms in {1,...,n} and 2w*x=3*y*z.
 * https://oeis.org/A212021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{⌊n²/3⌋} f(3t) * f(2t) where f(k) is the number
 *            of ordered pairs (x,y) with 1≤x,y≤n and x*y = k. This counts all
 *            quadruples (w,x,y,z)∈{1,…,n}⁴ satisfying 2*w*x = 3*y*z
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* If n=0 or 1 there are no valid tuples */
    if (n <= 1)
        return 0;

    /* Maximum product occurring in any pair */
    idx_t maxProd = n * n;

    /* Allocate array to store counts of each product */
    /* Use uint16_t: for n ≤ 10000 each value f(k) ≤ n ≤ 65535 */
    size_t sz = (size_t)maxProd + 1;
    uint16_t *freq = (uint16_t *)malloc(sz * sizeof *freq);
    if (!freq)
        return 0; /* allocation failed, return 0 as a safe value */

    /* Initialize to zero */
    memset(freq, 0, sz * sizeof *freq);

    /* Count all ordered pairs (x,y) with product ≤ maxProd */
    for (idx_t x = 1; x <= n; ++x) {
        for (idx_t y = 1; y <= n; ++y) {
            /* x*y cannot overflow idx_t for n up to 10⁹ (safe for this problem) */
            freq[(size_t)(x * y)]++;
        }
    }

    /* Sum over all admissible t */
    seq_t total = 0;
    idx_t maxT = maxProd / 3;              /* floor(n²/3) */
    for (idx_t t = 1; t <= maxT; ++t) {
        /* 3t and 2t are always ≤ maxProd by construction of maxT */
        uint32_t f1 = freq[(size_t)(3 * t)] ; /* f(3t) */
        uint32_t f2 = freq[(size_t)(2 * t)] ; /* f(2t) */
        total += (seq_t)f1 * f2;
    }

    free(freq);
    return total;
}
