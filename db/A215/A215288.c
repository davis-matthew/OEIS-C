/*
 * A215288  Number of permutations of 0..floor((n*4-1)/2) on even squares of
 *          an n X 4 array such that each row and column of even squares is
 *          increasing.
 * https://oeis.org/A215288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(⌊(n+1)/2⌋) * Catalan(⌊n/2⌋) * * binomial(2n,
 *            2⌊(n+1)/2⌋) * where Catalan(m) = binomial(2m, m) / (m+1) (m ≥ 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 36,
 *            big any n
 * Helpers:   binomial_iter
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial_iter.h"

static seq_t catalan(idx_t m)
{
    seq_t result = 1;
    /* C_0 = 1; recursion: C_{m+1} = C_m * (4m+2)/(m+2) */
    for (idx_t i = 0; i < m; ++i)
    {
        result = result * (seq_t)(4 * i + 2) / (seq_t)(i + 2);
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* f3 = ceil(n/2) = floor((n+1)/2), f4 = floor(n/2) */
    idx_t f3 = (n + 1) / 2;
    idx_t f4 = n / 2;

    seq_t cat3 = catalan(f3);
    seq_t cat4 = catalan(f4);

    seq_t bin = binomial_iter(2 * n, 2 * f3);

    return cat3 * cat4 * bin;
}
