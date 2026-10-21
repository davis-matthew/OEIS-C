/*
 * A082043  Square array, A(n, k) = (k*n)^2 + 2*k*n + 1, read by
 *          antidiagonals.
 * https://oeis.org/A082043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   A082043 is obtained by reading the array A(n,k) = ((n*k)+(1))^2
 *            in antidiagonal order. For index n (0‑based) we find the
 *            diagonal sum s and position offset inside that diagonal, then
 *            compute (row*col + 1)^2
 * Range:     u8 n <= 38, u16 n <= 542, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find diagonal s such that n < (s+1)(s+2)/2.
       Use the standard inverse of the triangular numbers. */
    idx_t s = (idx_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);

    /* Correct possible rounding errors from the double calculation. */
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    while (s * (s + 1) / 2 > n) --s;

    /* Position inside the diagonal. */
    idx_t offset = n - s * (s + 1) / 2;

    /* Coordinates in the array. */
    idx_t row = offset;
    idx_t col = s - offset;

    /* Value: (row*col + 1)^2. */
    seq_t prod  = (seq_t)row * (seq_t)col;
    seq_t base  = prod + 1;
    return base * base;
}
