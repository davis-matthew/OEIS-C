/*
 * A079621  Matrix square of unsigned Lah triangle abs(A008297(n,k)) or
 *          A105278(n,k).
 * https://oeis.org/A079621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   T_value, binomial_uint64, fact_ratio
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A079621: triangle of unsigned Lah numbers squared.
 *   a(n) = T(r,k) where r = smallest integer with r*(r+1)/2 >= n,
 *          k = n - r*(r-1)/2.
 *   T(r,k) = (r! / k!) * binomial(r-1, k-1) * 2^(r-k).
 *
 * The function computes the n-th term by determining its (r,k) pair
 * and multiplying the factors using only integer arithmetic.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/T_value.h"
#include "../utils/binomial_uint64.h"
#include "../utils/fact_ratio.h"

/* idx_t and seq_t are defined in the build environment. */

seq_t a(idx_t n)
{
    if (n == 0)   /* offset 1: a(1) is first term; values for n<1 are irrelevant */
        return 0;

    /* find the row r such that triangular(r-1) < n <= triangular(r) */
    idx_t r = 1;
    idx_t tri = 1;          /* triangular(r) = r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri += r;
    }

    /* k position within the row */
    idx_t prev_tri = tri - r;   /* triangular(r-1) */
    idx_t k = n - prev_tri;

    return T_value(r, k);
}
