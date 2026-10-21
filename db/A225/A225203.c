/*
 * A225203  Table T(n,k) composed of rows equal to: n * (the characteristic
 *          function of the multiples of (n+1)), read by downwards
 *          antidiagonals.
 * https://oeis.org/A225203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = i * ( (t+2)/(i+1) - (t+1)/(i+1) ) * where i = n -
 *            t*(t+1)/2 * t = ⌊((-1+√(8n-7))/2)⌋ * This is equivalent to the
 *            OEIS definition of A225203
 * Range:     u8 n <= 31, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* The helper function _floor_isqrt_u64 is available in the utils
   directory, but we do not use it here because we implement t via
   a binary search that avoids any 64‑bit overflow. */

/* Compute A225203(n) */
seq_t a(idx_t n)
{
    /* Find t such that t*(t+1)/2 < n <= (t+1)*(t+2)/2. */
    idx_t low = 0;
    idx_t high = 2 * n;      /* high is safe: (high)*(high+1)/2 >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = ((seq_t)mid * (mid + 1)) / 2;
        if (tri < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t t = low - 1;           /* largest t with tri(t) < n */

    /* Compute i = n - t*(t+1)/2 */
    seq_t tri_t = ((seq_t)t * (t + 1)) / 2;
    idx_t i = n - (idx_t)tri_t;

    /* Compute the difference of the two floor divisions */
    idx_t denom = i + 1;
    idx_t d1 = (t + 2) / denom;
    idx_t d2 = (t + 1) / denom;
    idx_t diff = d1 - d2;       /* 0 or 1 */

    return ((seq_t)i) * diff;
}
