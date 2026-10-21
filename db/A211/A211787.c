/*
 * A211787  Number of (w,x,y,z) with all terms in {1,...,n} and w*x<=2*y*z.
 * https://oeis.org/A211787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (w,x,y,z) ∈ [1,n]^4 with w*x ≤ 2*y*z
 *            Computation: Count the frequency of each product p = w*x, then
 *            for each such product add the number of pairs (y,z) with y*z ≥
 *            ceil(p/2). This is done by creating arrays of product
 *            frequencies and suffix sums of pair counts. Complexity O(n²)
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* maximum product value */
    idx_t prodmax = n * n;          /* fits in idx_t (uint64_t) */
    size_t sz = (size_t)(prodmax + 1);   /* number of array elements */

    /* frequency of each product w*x: use 16 bit because the maximum
       frequency ≤ n ≤ 10000 << 65535. */
    uint16_t *freq = (uint16_t *)malloc(sz * sizeof(uint16_t));
    if (!freq) return 0;
    memset(freq, 0, sz * sizeof(uint16_t));

    /* Count product frequencies for (w,x) */
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            idx_t p = w * x;
            freq[p]++;
        }
    }

    /* f[t] = number of pairs (y,z) with y*z ≥ t.
       We use a 32 bit array for these counts (≤ n² ≤ 1e8). */
    uint32_t *f = (uint32_t *)malloc((sz + 1) * sizeof(uint32_t));
    if (!f) { free(freq); return 0; }
    f[prodmax + 1] = 0;
    uint64_t running = 0;
    for (idx_t t = prodmax; t >= 1; --t) {
        running += freq[t];
        f[t] = (uint32_t)running;
    }

    /* Compute the answer */
    seq_t ans = 0;
    for (idx_t p = 1; p <= prodmax; ++p) {
        idx_t thr = (p + 1) / 2;          /* ceil(p/2) */
        ans += (seq_t)freq[p] * (seq_t)f[thr];
    }

    free(freq);
    free(f);
    return ans;
}
