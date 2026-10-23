/*
 * A131779  Triangle read by rows: T(n,k) = 2*A065941(n-1,k-1) - (-1)^(n+k).
 * https://oeis.org/A131779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = smallest integer with r(r+1)/2 >= n
 *            (row), c = n - (r-1)r/2 (column within that row). T(r,c) = 2 *
 *            binomial(r-1-floor(c/2), floor((c-1)/2)) - (-1)^(r+c) The
 *            triangle is read row by row with offset 1
 * Range:     u8 n <= 111, u16 n <= 362, u32 n <= 1250, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Multiply-then-divide binomial helper that works for any seq_t size. */
static seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;                /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide to keep integers */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* offset 1; for safety return 0 for n == 0. */
    if (n == 0) return 0;

    /* find row r: smallest r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* column index within this row (1‑based) */
    idx_t col = n - (r - 1) * r / 2;

    /* parameters for the binomial coefficient */
    idx_t k_half = col / 2;              /* floor(col/2)             */
    idx_t j       = (col - 1) / 2;        /* floor((col-1)/2)         */
    idx_t m       = (r - 1) - k_half;    /* n-1-floor(k/2)           */

    seq_t coeff = binomial_small(m, j);

    /* T(r,col) = 2*coeff - (-1)^(r+col) */
    seq_t val = coeff * 2;
    if ((r + col) & 1)            /* odd exponent: subtract -1 => add 1 */
        val += 1;
    else                          /* even exponent: subtract +1      */
        val -= 1;

    return val;
}
