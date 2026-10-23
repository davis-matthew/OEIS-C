/*
 * A163672  Triangle T(n,m) = 2mn + m + n + 7 read by rows.
 * https://oeis.org/A163672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(row, col) where * T(r, c) = 2*r*c + r + c + 7 * * The
 *            linear index n is converted to (row, col) by reading the *
 *            infinite triangular array row by row: * r = smallest integer
 *            such that r*(r+1)/2 >= n * c = n - (r-1)*r/2 * (row, col) are
 *            1‑based. This implements A163672
 * Range:     u8 n <= 65, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) /* n >= 1 */
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t low = 1, high = 1;
    while (1) {
        unsigned __int128 tri = (unsigned __int128)high * (high + 1) / 2;
        if (tri >= n) break;
        if (high == UINT64_MAX) break;    /* safety, should not happen */
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n) high = mid;
        else          low  = mid + 1;
    }
    idx_t r = low;

    /* Column index */
    unsigned __int128 prev_tri = (unsigned __int128)(r - 1) * r / 2;
    idx_t c = n - (idx_t)prev_tri;

    /* Compute the triangle value in seq_t */
    seq_t row = (seq_t)r;
    seq_t col = (seq_t)c;
    seq_t result = ((seq_t)2 * row * col) + row + col + (seq_t)7;
    return result;
}
