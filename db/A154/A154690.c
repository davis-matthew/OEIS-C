/*
 * A154690  Triangle read by rows: T(n, k) = (2^(n-k) + 2^k)*binomial(n,k), 0
 *          <= k <= n.
 * https://oeis.org/A154690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(1+8n)-1)/2) k = n - r(r+1)/2
 *            T(r,k) = (2^(r-k)+2^k) * C(r,k) (C is binomial coefficient)
 * Range:     u8 n <= 7, u16 n <= 79, u32 n <= 258, u64 n <= 956,
 *            u128 n <= 1325, big any n
 * Helpers:   binom, floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    // Compute r = floor((sqrt(1+8n)-1)/2)
    seq_t tmp = (seq_t)n * 8 + 1;
    seq_t sqrt_tmp = floor_sqrt(tmp);
    idx_t r = (idx_t)((sqrt_tmp - 1) >> 1);

    // Starting index of row r
    idx_t start = (r * (r + 1)) >> 1;

    idx_t k = n - start;          // column within the row

    seq_t pow_left  = pow2((idx_t)(r - k));
    seq_t pow_right = pow2(k);
    seq_t binom_val = binom(r, k);

    return (pow_left + pow_right) * binom_val;
}
