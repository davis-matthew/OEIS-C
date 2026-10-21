/*
 * A212056  Number of (w,x,y,z) with all terms in {1,...,n} and w*x>y*z+2.
 * https://oeis.org/A212056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p=1}^{n^2} f[p] * G[p-3], where f[p] = #{(w,x)∈[1,n]^2
 *            : w*x = p} G[t] = #{(y,z)∈[1,n]^2 : y*z ≤ t} The code counts
 *            each product p once, stores its frequency in a table, then
 *            iterates over all products computing the cumulative count G[p-3]
 *            on the fly
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;

    idx_t limit = n * n;               /* maximum product */
    /* allocate frequency table; 16‑bit is enough for any n≤10000 */
    uint16_t *freq = (uint16_t *)malloc((limit + 1) * sizeof(uint16_t));
    if (!freq) return 0;
    memset(freq, 0, (limit + 1) * sizeof(uint16_t));

    /* count products w*x */
    for (idx_t w = (idx_t)1; w <= n; ++w)
        for (idx_t x = (idx_t)1; x <= n; ++x)
            ++freq[w * x];

    seq_t ans = 0;
    seq_t cum = 0; /* running total of G[p-3] */

    for (idx_t p = (idx_t)1; p <= limit; ++p) {
        if (p >= (idx_t)4)
            cum += (seq_t)freq[p - 3];
        ans += (seq_t)freq[p] * cum;
    }

    free(freq);
    return ans;
}
