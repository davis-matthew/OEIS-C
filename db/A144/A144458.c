/*
 * A144458  Two sequence determinant triangle sequence: a(n)=A000045(n);
 *          b(n)=b(n-1)+b(n-2)+b(n-3) :2 start;A141036(n);
 *          t(n,m)=t(n,m)=a(m)*b(n)-b(m)*a(n).
 * https://oeis.org/A144458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   The sequence A144458 is given by the flattened * two–sequence
 *            determinant triangle: * * a(r) = Fibonacci(r) (a(0)=0, a(1)=1,
 *            a(2)=1,…) * b(r) = Tribonacci(r) (b(0)=2, b(1)=1, b(2)=1) *
 *            t(r,m) = a(m) * b(r) - b(m) * a(r) for 0 ≤ m < r * * The
 *            sequence is obtained by listing the entries of this * t...
 * Range:     i8 n <= 40, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find the row r that contains the element number n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* previous cumulative count of elements before row r */
    idx_t prev = (r - 1) * r / 2;
    /* column index in this row, zero‑based */
    idx_t m = n - prev - 1;

    /* compute Fibonacci numbers up to r (0‑based indices) */
    seq_t fib_prev2 = 0;   /* F(i-2)   */
    seq_t fib_prev1 = 1;   /* F(i-1)   */
    seq_t fib_val   = 0;   /* F(i)     */
    seq_t fib_m  = 0, fib_r = 0;

    for (idx_t i = 0; i <= r; ++i) {
        if (i == 0)
            fib_val = 0;
        else if (i == 1)
            fib_val = 1;
        else {
            fib_val = fib_prev1 + fib_prev2;
            fib_prev2 = fib_prev1;
            fib_prev1 = fib_val;
        }
        if (i == m)
            fib_m = fib_val;
        if (i == r)
            fib_r = fib_val;
    }

    /* compute Tribonacci numbers up to r (0‑based indices) */
    seq_t trib0 = 2, trib1 = 1, trib2 = 1;
    seq_t trib_prev3 = trib0; /* T(i-3)   */
    seq_t trib_prev2 = trib1; /* T(i-2)   */
    seq_t trib_prev1 = trib2; /* T(i-1)   */
    seq_t trib_val   = 0;     /* T(i)     */
    seq_t trib_m = 0, trib_r = 0;

    for (idx_t i = 0; i <= r; ++i) {
        if (i == 0)
            trib_val = trib0;
        else if (i == 1)
            trib_val = trib1;
        else if (i == 2)
            trib_val = trib2;
        else {
            trib_val = trib_prev1 + trib_prev2 + trib_prev3;
            trib_prev3 = trib_prev2;
            trib_prev2 = trib_prev1;
            trib_prev1 = trib_val;
        }
        if (i == m)
            trib_m = trib_val;
        if (i == r)
            trib_r = trib_val;
    }

    /* determinant value for the requested element */
    return fib_m * trib_r - trib_m * fib_r;
}
