/*
 * A095704  Triangle read by rows giving coefficients of the trigonometric
 *          expansion of sin(n*x).
 * https://oeis.org/A095704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = binomial(row, col+1) * sin(pi*(col+1)/2) * where row =
 *            smallest integer r with r*(r+1)/2 >= n * and col = n -
 *            (row-1)*row/2 - 1 (0‑based column within the row) *
 *            sin(pi*(k+1)/2) is 0 for k+1 ≡ 0 or 2 (mod 4), * +1 for k+1 ≡ 1
 *            (mod 4), and -1 for k+1 ≡ 3 (mod 4)
 * Range:     i8 n <= 49, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r = r * (seq_t)(n - k + i) / (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* sequence starts at offset 1 */

    /* find row */
    idx_t row = (idx_t)((sqrt((long double)(8.0L * n + 1.0L)) - 1.0L) / 2.0L);
    while ((__uint128_t)row * (row + 1) / 2 < n) ++row;
    while (row > 1 && (__uint128_t)(row - 1) * row / 2 >= n) --row;

    /* first index of this row */
    idx_t start = (row - 1) * row / 2 + 1;
    idx_t col = n - start;                 /* 0‑based column */

    idx_t k = col + 1;                      /* binomial uses 1‑based k+1 */
    int mod = k & 3;
    if (mod == 1) {
        return binom(row, k);
    } else if (mod == 3) {
        return -binom(row, k);
    } else {
        return 0;
    }
}
