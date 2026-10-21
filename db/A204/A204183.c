/*
 * A204183  Symmetric matrix based on f(i,j) defined by f(i,1)=f(1,j)=1;
 *          f(i,i)= (-1)^(i-1); f(i,j)=0 otherwise; by antidiagonals.
 * https://oeis.org/A204183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   * Let s = minimal integer with s*(s-1)/2 >= n (the diagonal sum
 *            i+j), * let before = (s-2)*(s-1)/2 (entries before that
 *            diagonal), * let d = n - before - 1 (offset within the
 *            diagonal), * let i = d + 1, * let j = s - i. * Then a(n) = 1 if
 *            i==1 or j==1, * (-1)^(i-1) if i==j, * 0 otherwise
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* for safety, though n>=1 by specification */

    /* Find minimal s such that s*(s-1)/2 >= n */
    idx_t low = 1;
    idx_t high = 2;
    while (((__int128)high * (high - 1)) / 2 < n)
        high <<= 1;               /* exponential search for upper bound */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (((__int128)mid * (mid - 1)) / 2 < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t s = low;                  /* sum i + j */

    /* Entries preceding the diagonal s */
    idx_t before = ((s >= 2)
                    ? (((__int128)(s - 2) * (s - 1)) / 2)
                    : 0);

    /* Position within the diagonal */
    idx_t d = n - before - 1;       /* 0-based offset */

    idx_t i = d + 1;                /* row index */
    idx_t j = s - i;                /* column index */

    /* Determine the matrix value */
    if (i == 1 || j == 1) {
        return 1;
    } else if (i == j) {
        /* (-1)^(i-1) */
        return (((i - 1) & 1) == 0) ? 1 : -1;
    } else {
        return 0;
    }
}
