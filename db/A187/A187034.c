/*
 * A187034  Number triangle T(n,k) = (-1)^(n-k) if binomial(k, n-k) > 0, 0
 *          otherwise, with 0 <= k <= n.
 * https://oeis.org/A187034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the (row, col) entry of the triangle T(row,col) where T
 *            = (-1)^(row-col) if row <= 2*col otherwise 0. The sequence is
 *            enumerated row by row (row0 col0, row1 col0,row1 col1, row2
 *            col0,...)
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 64‑bit unsigned number.
   Returns floor(sqrt(x)).  Uses only integer arithmetic. */
static inline uint64_t _isqrt_u64(uint64_t x)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;          /* largest power of 4 <= 2^63 */
    while (bit > x) bit >>= 2;          /* bring bit <= x */
    while (bit) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       Using floor(sqrt(1+8*n)) to compute r. */
    uint64_t m = (uint64_t)8 * n + 1;
    uint64_t t = _isqrt_u64(m);          /* floor(sqrt(1+8*n)) */
    uint64_t r = (t - 1) / 2;            /* row number */

    /* Offset of the first element of this row. */
    uint64_t offset = (r * (r + 1)) / 2;

    /* Column within the row. */
    uint64_t col = n - offset;

    /* Check if the triangle entry is non‑zero. */
    if (r <= 2 * col) {
        /* Sign is (-1)^(r-col). */
        if (((r - col) & 1ULL) != 0)
            return (seq_t)(-1);
        else
            return (seq_t)1;
    }
    return (seq_t)0;
}
