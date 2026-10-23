/*
 * A126064  Triangle read by rows, obtained by multiplying columns of triangle
 *          in A094587 by 1,2,4,8,16,... respectively.
 * https://oeis.org/A126064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   Let r be the unique integer satisfying r(r+1)/2 <= n <
 *            (r+1)(r+2)/2. Let k = n - r(r+1)/2. Then a(n) = binomial(r,k) *
 *            (r - k)! * 2^k
 * Range:     u8 n <= 20, u16 n <= 36, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned __int128 triangular(idx_t x)
{
    return (unsigned __int128)x * (x + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Find row r by binary search on the triangular numbers */
    idx_t low = 0, high = 1;
    while (triangular(high) <= (unsigned __int128)n)
        high <<= 1;                 /* double until we overshoot */

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;  /* upper mid */
        if (triangular(mid) <= (unsigned __int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Column index k within the row */
    idx_t row_start = (idx_t)(triangular(r));
    idx_t k = n - row_start;

    /* Compute binomial(r,k) */
    seq_t binom = 1;
    for (idx_t i = 1; i <= k; ++i)
        binom = binom * (r - k + i) / i;  /* exact division */

    /* Compute (r-k)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= r - k; ++i)
        fact *= i;

    /* Compute 2^k */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow2 *= 2;

    /* Combine all parts */
    return binom * fact * pow2;
}
