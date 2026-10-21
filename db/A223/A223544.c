/*
 * A223544  T(n, k) = n*k - 1.
 * https://oeis.org/A223544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = r * k - 1 * where * r = floor((sqrt(8*n-7) + 1)/2), * k =
 *            n - r*(r-1)/2 (1 ≤ k ≤ r) * * The function computes the n‑th
 *            term of triangle T(n,k) = n*k - 1 * read by rows (offset 1)
 * Range:     u8 n <= 8, u16 n <= 1925, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Use seq_t for all arithmetic to preserve arbitrary precision. */
    seq_t eight = (seq_t)8;
    seq_t seven = (seq_t)7;
    seq_t one   = (seq_t)1;
    seq_t two   = (seq_t)2;

    /* discriminant 8*n - 7 */
    seq_t disc  = eight * (seq_t)n - seven;
    /* integer sqrt of disc */
    seq_t s     = floor_sqrt(disc);
    /* t = r - 1 */
    seq_t t     = (s - one) / two;
    seq_t r     = t + one;
    /* sum of first r-1 natural numbers */
    seq_t sum_prev = r * (r - one) / two;
    /* position k within row r */
    seq_t k     = (seq_t)n - sum_prev;
    /* value T(r,k) = r*k - 1 */
    seq_t ans   = r * k - one;
    return ans;
}
