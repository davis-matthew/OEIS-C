/*
 * A213761  Rectangular array: (row n) = b**c, where b(h) = h, c(h) =
 *          3*n-5+3*h, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) reads the array T(r,c) with T(r,c)=c*(c+1)*(3*r+c-3)/2 by
 *            antidiagonal enumeration
 * Range:     u8 n <= 28, u16 n <= 1035, u32 n <= 1035, u64 n <= 1035,
 *            u128 n <= 1035, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Convert 1‑based index to 0‑based */
    idx_t idx = n - 1;

    /* Find the antidiagonal sum s such that (s-1)*s/2 > idx. */
    idx_t low = 2, high = idx + 2;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned long long t = ((unsigned long long)(mid - 1) * mid) / 2;
        if (t > idx)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Number of terms before this diagonal */
    unsigned long long prev_ull = ((unsigned long long)(s - 2) * (s - 1)) / 2;
    idx_t offset = idx - (idx_t)prev_ull;   /* 0‑based offset on this diagonal */

    /* Coordinates on the diagonal: (row, col) = (offset+1, s-(offset+1)) */
    idx_t row = offset + 1;
    idx_t col = s - row;

    /* Compute T(row,col) = col*(col+1)*(3*row+col-3)/2 */
    unsigned long long k = (unsigned long long)col;
    unsigned long long r = (unsigned long long)row;
    unsigned long long part1 = k * (k + 1) / 2;    /* col*(col+1)/2 */
    unsigned long long part2 = 3 * r + k - 3;      /* 3*row + col - 3 */
    unsigned long long val  = part1 * part2;       /* final value */

    return (seq_t)val;
}
