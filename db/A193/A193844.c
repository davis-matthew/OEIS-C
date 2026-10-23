/*
 * A193844  Triangular array: the fission of ((x+1)^n) by ((x+1)^n); i.e., the
 *          self-fission of Pascal's triangle.
 * https://oeis.org/A193844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For an index n, let r be the largest integer such that r(r+1)/2
 *            ≤ n. Let k = n – r(r+1)/2. Then a(n) = T(r,k) where T(r,k) =
 *            Σ_{i=0}^{k} (-1)^i * binomial(r+1, k-i) * 2^(k-i). The binomial
 *            and power are computed with the helper functions provided in
 *            utils
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"  /* binomial(idx_t n, idx_t k) → seq_t */
#include "../utils/pow2.h"      /* pow2(idx_t exp) → seq_t */

/* Integer square root: floor(sqrt(x)) */
static idx_t integer_sqrt(idx_t x)
{
    idx_t low = 0;
    idx_t high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid != 0 && mid > x / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Find r = floor((sqrt(8*n+1)-1)/2) */
    idx_t s = integer_sqrt((idx_t)1 + (idx_t)8 * n);
    idx_t r = (s - 1) >> 1;               /* (s-1)/2 */

    /* Compute the offset of the first element of row r */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;                 /* 0 ≤ k ≤ r */

    seq_t sum = 0;
    for (idx_t i = 0; i <= k; ++i) {
        seq_t bin = binomial(r + 1, k - i);
        seq_t pow = pow2(k - i);
        seq_t term = bin * pow;
        if ((i & 1u) == 0)
            sum += term;
        else
            sum -= term;
    }
    return sum;
}
