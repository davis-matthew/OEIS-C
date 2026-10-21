/*
 * A212143  Number of (w,x,y,z) with all terms in {1,...,n} and w <=
 *          (geometric mean of x,y,z).
 * https://oeis.org/A212143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (w,x,y,z)∈[1,n]^4 with w^3 ≤ x·y·z
 *            (i.e., w ≤ the geometric mean of x,y,z)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Size of product range for pairs (x,y) */
    size_t N = (size_t)n;
    size_t M = N * N;                /* maximum value of x*y */
    size_t SZ = M + 1;               /* array indices 0..M */

    /* Frequency array for exact pair products */
    uint32_t *freq = (uint32_t *)calloc(SZ, sizeof(uint32_t));
    if (!freq) return 0;             /* allocation failure → undefined behavior */

    /* Count exact products of pairs (x,y) */
    for (idx_t x = 1; x <= n; ++x) {
        size_t sx = (size_t)x;
        for (idx_t y = 1; y <= n; ++y) {
            size_t p = sx * (size_t)y;
            ++freq[p];
        }
    }

    /* Convert to suffix counts: freq[t] = number of pairs with product ≥ t */
    for (size_t t = M; t > 0; --t)
        freq[t - 1] += freq[t];

    seq_t result = 0;

    /* Summation over w and z */
    for (idx_t w = 1; w <= n; ++w) {
        uint64_t w3 = (uint64_t)w * w * w;   /* w^3 fits in 64‑bit for n <= 10000 */

        for (idx_t z = 1; z <= n; ++z) {
            uint64_t t_val = (w3 + (uint64_t)z - 1) / z;  /* ceil(w^3 / z) */
            if (t_val > M) continue;                    /* no pair product can reach it */
            size_t idx = (size_t)t_val;
            result += (seq_t)freq[idx];
        }
    }

    free(freq);
    return result;
}
