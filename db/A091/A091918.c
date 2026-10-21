/*
 * A091918  Inverse of number triangle A091917.
 * https://oeis.org/A091918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 << k if n = k*(k+1)/2 binomial(k, n - k*(k+1)/2)
 *            otherwise where k = floor((sqrt(8*n+1)-1)/2) (triangular index
 *            of n)
 * Range:     u8 n <= 35, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   binomial_k_r
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_k_r.h"

static idx_t triangular_row(idx_t n)

/* Find the largest k such that k*(k+1)/2 <= n */
{
    idx_t low = 0, high = 1;
    while ((high * (high + 1)) / 2 <= n) high <<= 1;
    while (low < high) {
        idx_t mid = (low + high + 1) / 2;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t k = triangular_row(n);
    idx_t tri = (k * (k + 1)) / 2;
    idx_t r = n - tri;

    if (r == 0u) {
        /* compute 2^k safely using 128‑bit and then cast */
        unsigned __int128 val = 1;
        for (idx_t i = 0; i < k; ++i) val <<= 1;
        return (seq_t)val;
    } else {
        return binomial_k_r(k, r);
    }
}
