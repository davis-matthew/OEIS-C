/*
 * A230419  Square array A(n,k) = difference of digit sums in factorial base
 *          representations (A007623) of n and k, n>=0, k>=0, read by
 *          antidiagonals; A(n,k) = A034968(n)-A034968(k).
 * https://oeis.org/A230419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, tabl
 * Formula:   a(n) = A034968(col(n)) - A034968(row(n)) where
 *            (col,row)=antidiagonal_coords(n)
 * Range:     i8 n <= 7380, i16 n <= 7380, i32 n <= 7380, i64 n <= 7380,
 *            i128 n <= 7380, big any n
 * Validated: standard, pass, 94 terms through n = 7332 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

/* Compute the sum of digits of n in factorial base (factoradic representation). */
static seq_t factoradic_digit_sum(idx_t n)
{
    seq_t sum = 0;
    idx_t base = 2;           /* first factorial base place is 1! = 1, divisor 2 */
    while (n > 0)
    {
        seq_t digit = (seq_t)(n % base);
        sum += digit;
        n /= base;
        ++base;               /* next place is (i+1)! -> divisor increases by 1 */
    }
    return sum;
}

/* Sequence A230419 */
seq_t a(idx_t n)
{
    idx_t row, col;
    antidiagonal_coords(n, &row, &col);          /* row = i, col = j as per OEIS definition */

    seq_t sum_col = factoradic_digit_sum(col);
    seq_t sum_row = factoradic_digit_sum(row);

    return sum_col - sum_row;
}
