/*
 * A140880  Triangle read by rows, T(n,k) =
 *          Gamma(n+3)/(Gamma(k+1)*Gamma(n-k+1)) for n>=0 and 0<=k<=n.
 * https://oeis.org/A140880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = Gamma(n+3)/(Gamma(k+1)*Gamma(n-k+1)) * a(n) = triangle
 *            read by rows: a(index) = T(row, col) * where row r satisfies
 *            r(r+1)/2 ≤ index < (r+1)(r+2)/2 * and col = index - r(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom_n_k, floor_isqrt_u64, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_isqrt_u64.h"
#include "../utils/binom_n_k.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Compute the row number r */
    idx_t r = (floor_isqrt_u64((idx_t)8 * n + 1) - 1) / 2;

    /* Position within the row */
    idx_t triangular = r * (r + 1) / 2;  /* number of elements before row r */
    idx_t k = n - triangular;           /* column index 0 .. r */

    /* Binomial part */
    seq_t bin = binom_n_k(r + 2, k);

    /* Multiply by (r+2-k)*(r+1-k) */
    seq_t res = mul_small(bin, (unsigned)(r + 2 - k));
    res = mul_small(res, (unsigned)(r + 1 - k));

    return res;
}
