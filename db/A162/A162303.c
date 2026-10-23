/*
 * A162303  Product matrix [C(k,n-k)]*A001263.
 * https://oeis.org/A162303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the greatest integer such that
 *            r*(r+1)/2 ≤ n, k = n – r*(r+1)/2, and T(r,k) = Σ_{j=0..r}
 *            C(j,r−j) * C(j+1,k) * C(j+1,k+1) / (j+1)
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                 /* for idx_t, seq_t types if needed */
#include <math.h>                   /* sqrt */
#include "../utils/binom.h"        /* binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* find r with r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* k = n - triangular(r) */
    idx_t before = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - before;

    seq_t sum = 0;

    for (idx_t j = 0; j <= r; ++j) {
        seq_t c1 = binom(j, r - j);
        if (!c1) continue;

        seq_t c2 = binom(j + 1, k);
        if (!c2) continue;

        seq_t c3 = binom(j + 1, k + 1);
        if (!c3) continue;

        seq_t term = c1 * c2 * c3;
        term = term / (seq_t)(j + 1);   /* exact division */
        sum += term;
    }

    return sum;
}
