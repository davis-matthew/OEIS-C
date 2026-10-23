/*
 * A144405  Triangle T(n,k) = binomial(n, k)*(3*binomial(n, k)^2 - binomial(n,
 *          k) - 1), read by rows.
 * https://oeis.org/A144405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where row = floor((sqrt(8*n + 1) - 1)/2), col
 *            = n - row*(row + 1)/2, T(row, col) = binomial(row, col) *
 *            (3*binomial(row, col)^2 - binomial(row, col) - 1)
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 95, u64 n <= 309,
 *            u128 n <= 1100, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the utility function binom(idx_t a, idx_t b) */

#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/binom.h"

static inline idx_t isqrt_u64(idx_t n)
{
    /* integer square root by binary search */
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Find the row index */
    idx_t sqrt_term = isqrt_u64(8 * n + 1);
    idx_t row = (sqrt_term - 1) >> 1;

    /* Starting offset of this row */
    seq_t offset = ((seq_t)row * (row + 1)) / 2;

    /* Column index */
    idx_t col = (idx_t)(n - (idx_t)offset);

    /* Binomial coefficient */
    seq_t b = binom(row, col);

    /* Compute T(row, col) */
    seq_t b2 = b * b;                /* b^2 */
    seq_t part = b2 * 3 - b - 1;     /* 3*b^2 - b - 1 */
    seq_t result = b * part;         /* b * (3*b^2 - b - 1) */

    return result;
}
