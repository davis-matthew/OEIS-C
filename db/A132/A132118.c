/*
 * A132118  Triangle read by rows: T(n,k) = n*(n-1)/2 + 2*k - 1.
 * https://oeis.org/A132118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = r*(r-1)/2 + 2*j - 1 * where r is the row number (smallest
 *            r with r*(r+1)/2 >= n) * and j = n - (r-1)*r/2 is the column
 *            index in that row
 * Range:     u8 n <= 7, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined; guard for safety */
    seq_t nn = n;                   /* promote index to seq_t */

    /* preliminary estimate of r using integer sqrt */
    seq_t val = nn * (seq_t)8 + 1;
    seq_t s   = floor_sqrt(val);
    seq_t r   = (s - 1) / 2;        /* floor approx */

    /* correct upward if needed */
    seq_t r_sum = r * (r + 1) / 2;
    while (r_sum < nn) {
        ++r;
        r_sum = r * (r + 1) / 2;
    }

    /* correct downward if we overshot */
    while (r > 0 && ((r - 1) * r / 2) >= nn) {
        --r;
    }

    seq_t prev_sum = (r - 1) * r / 2;
    seq_t j        = nn - prev_sum;        /* 1 ≤ j ≤ r */

    seq_t result = r * (r - 1) / 2 + (j << 1) - 1;
    return result;
}
