/*
 * A138585  The sequence is formed by concatenating subsequences S1, S2, ...
 *          each of finite length. S1 consists of the element 1. The n-th
 *          subsequence consist of numbers {(n/2)*(n/2 - 1)+ 1, ...,
 *          (n/2)*(n/2 + 1)} for n even, {((n-1)/2)^2, ..., (n-1)/2 * ((n-1)/2
 *          + 2)} for n odd.
 * https://oeis.org/A138585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = start_val(r(n)) + (n - offset_row(r(n))), where * r(n) =
 *            minimal r with r*(r+1)/2 ≥ n. * For odd r > 1: start_val =
 *            ((r-1)/2)^2. * For even r: start_val = (r/2)*((r/2)-1) + 1. *
 *            Special case: a(1) = 1
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* Required for uint64_t if used by the build */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;           /* Special first term */

    /* Find the row r that contains position n */
    idx_t r = 1;
    while (((seq_t)r * (r + 1)) / 2 < n)
        ++r;

    /* Compute the first linear index of this row and the offset of n */
    seq_t row_start = ((seq_t)(r - 1) * r) / 2 + 1;
    seq_t offset    = (seq_t)n - row_start;

    /* Compute the first value of the subsequence in this row */
    seq_t start_val;
    if (r & 1) {                       /* odd r (except r==1, handled above) */
        idx_t k = (r - 1) / 2;
        start_val = (seq_t)k * k;
    } else {                           /* even r */
        idx_t k = r / 2;
        start_val = (seq_t)k * (k - 1) + 1;
    }

    return start_val + offset;
}
