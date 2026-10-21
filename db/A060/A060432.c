/*
 * A060432  Partial sums of A002024.
 * https://oeis.org/A060432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((6*n+1)*R - R^3)/6, where R = round(sqrt(2*n))
 * Range:     u8 n <= 12, u16 n <= 546, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Computes OEIS A060432: Partial sums of A002024

#include <stdint.h>

/* Integer square root (floor) for unsigned 64-bit integers */
static idx_t isqrt_idx(idx_t v)
{
    idx_t low = 0;
    idx_t high = v;
    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        if (mid != 0 && mid > v / mid)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t two_n = 2 * n;
    idx_t sq = isqrt_idx(two_n);          /* floor(sqrt(2*n)) */
    idx_t rem = two_n - sq * sq;          /* remainder: 2*n - sq^2 */
    idx_t inc = (rem >= sq + 1) ? 1 : 0;    /* round up if needed */
    seq_t R = (seq_t)(sq + inc);            /* round(sqrt(2*n)) */

    /* Compute numerator: (6*n + 1) * R - R^3 */
    seq_t temp = (seq_t)(6 * n + 1) * R;
    seq_t r2 = R * R;
    seq_t r3 = r2 * R;
    seq_t numer = temp - r3;

    return numer / 6;        /* division is exact */
}
