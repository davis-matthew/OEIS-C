/*
 * A221975  Triangle read by rows of the numbers that are the sum of some
 *          consecutive Mersenne numbers A000225.
 * https://oeis.org/A221975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(r-k+1) * (2^k - 1) - k where r is the smallest integer
 *            with r(r+1)/2 ≥ n, and k = n - (r-1)r/2 (1 ≤ k ≤ r)
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"  /* seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≥ n  (r ≥ 1) */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Position within the row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;  /* 1‑based index in row r */

    /* Compute the powers of 2 needed */
    seq_t pow_start = pow2(r - k + 1);
    seq_t pow_k     = pow2(k);

    /* a(n) = pow_start * (pow_k - 1) - k */
    seq_t res = pow_start * (pow_k - (seq_t)1) - (seq_t)k;
    return res;
}
