/*
 * A202064  Triangle T(n,k), read by rows, given by (2, -1/2, 1/2, 0, 0, 0, 0,
 *          0, 0, 0, ...) DELTA (0, 1/2, -1/2, 0, 0, 0, 0, 0, 0, 0, ...) where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A202064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with r = max{m | m(m+1)/2 ≤ n}, k = n – r(r+1)/2
 *            and T(r,k) = binomial(r+1, 2k+1)
 * Range:     u8 n <= 37, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The binomial routine is provided by the build system
    (#include "../utils/binomial.h" for “binomial”). */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Determine column k within that row. */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Compute the triangle entry. */
    return binomial(r + 1, 2 * k + 1);
}
