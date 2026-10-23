/*
 * A133341  A007318 * A134312.
 * https://oeis.org/A133341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Formula implemented:  a(n) is the (r,c)th entry of a triangular array where
 *  r = floor((√(8n+1)-1)/2), c = n - r(r+1)/2, 0 ≤ c ≤ r.
 *  T(r,c) = binom(r+1, c+1) * 2^(max(0, c-1))
 *
 *  This gives the flattened triangle
 *      1;
 *      2,1;
 *      3,3,2;
 *      4,6,8,4;
 *      5,10,20,20,8;
 *      …
 *  as defined in OEIS A133341.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r: r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index c */
    idx_t c = n - r * (r + 1) / 2;

    /* Compute binom(r+1, c+1) */
    idx_t N = r + 1;
    idx_t k = c + 1;
    seq_t bin = 1;
    for (idx_t i = 1; i <= k; ++i) {
        bin *= (seq_t)(N - i + 1);
        bin /= (seq_t)i;
    }

    /* Compute 2^(c-1) for c > 0, factor = 1 for c == 0 */
    seq_t factor = 1;
    if (c > 0) {
        for (idx_t i = 1; i <= c - 1; ++i)
            factor *= (seq_t)2;
    }

    return bin * factor;
}
