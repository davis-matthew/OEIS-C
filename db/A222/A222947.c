/*
 * A222947  Number of distinct sums i+j+k with |i|, |j|, |k|, |i*j*k| <= n and
 *          gcd(i,j,k) <= 1.
 * https://oeis.org/A222947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct sums i+j+k with |i|,|j|,|k|,|i*j*k| <=
 *            n and gcd(i,j,k)<=1
 * Range:     u8 n <= 64, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   gcd_uint64_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_uint64_uint64.h"

seq_t a(idx_t n)
{
    /* Work with signed 64‑bit indices for loops */
    int64_t nn = (int64_t)n;
    int64_t offset = 3 * nn;               /* map sum in [-3n,3n] to [0,6n] */
    size_t sz = (size_t)(6 * nn + 1);
    bool *seen = (bool *)malloc(sz);
    if (!seen) return 0;
    memset(seen, 0, sz);

    for (int64_t i = -nn; i <= nn; ++i) {
        uint64_t abs_i = (i >= 0) ? (uint64_t)i : (uint64_t)(-i);
        uint64_t maxJ = (abs_i == 0) ? nn : n / abs_i;   /* |j| <= n/|i| if |i|>0 */
        for (int64_t j = -(int64_t)maxJ; j <= (int64_t)maxJ; ++j) {
            uint64_t abs_j = (j >= 0) ? (uint64_t)j : (uint64_t)(-j);
            uint64_t prod = abs_i * abs_j;
            uint64_t limitK = (prod == 0) ? n : n / prod;                 /* |k| <= n/|i*j| */
            for (int64_t k = -(int64_t)limitK; k <= (int64_t)limitK; ++k) {
                uint64_t abs_k = (k >= 0) ? (uint64_t)k : (uint64_t)(-k);
                uint64_t g = gcd_uint64_uint64(abs_i, abs_j);
                g = gcd_uint64_uint64(g, abs_k);
                if (g <= 1) {
                    int64_t sum = i + j + k;                       /* sum ∈ [-3n,3n] */
                    seen[(size_t)(sum + offset)] = 1;
                }
            }
        }
    }

    seq_t count = 0;
    for (size_t idx = 0; idx < sz; ++idx)
        if (seen[idx]) ++count;

    free(seen);
    return count;
}
