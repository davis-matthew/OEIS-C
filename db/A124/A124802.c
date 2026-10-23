/*
 * A124802  Triangle, row sums = Fibonacci numbers in two ways, companion to
 *          A124801.
 * https://oeis.org/A124802
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  tabl, sign
 * Formula:   a(n) = 1 if the element is the first in its row; * otherwise *
 *            a(n) = (-1)^k * Fib(k‑1) * C(r, k), * where r is the row
 *            containing the element (r ≥ 1), * k is its 1‑based column in
 *            that row (k ≥ 2), * Fib is the Fibonacci function (Fib(1)=1,
 *            Fib(2)=1), * and C(r, k) is the binomial coefficient
 * Range:     i8 n <= 31, i16 n <= 37, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes required helper functions. */
#include "../utils/fib.h"       /* seq_t fib(idx_t n); */
#include "../utils/binomial.h" /* seq_t binomial(idx_t n, idx_t k); */

seq_t a(idx_t n)
{
    /* Offset is 2: a(2) is the first element of the triangle. */
    if (n < 2)                /* behavior for n < 2 is undefined */
        return (seq_t)0;

    /* Zero‑based index within the flattened triangle. */
    idx_t m = n - 2;

    /* Find the row r such that t_{r-1} <= m < t_r,
     * where t_r = r*(r+1)/2 is the total number of elements up to row r. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 <= m)
        ++r;

    /* Index of the first element of the found row (0‑based). */
    idx_t t_prev = ((r - 1) * r) / 2;

    /* 1‑based column within the row. */
    idx_t k = m - t_prev + 1;

    if (k == 1)              /* first column is always 1 */
        return (seq_t)1;

    /* Compute Fibonacci(k-1) and binomial(r, k). */
    seq_t fib_val = fib(k - 1);          /* k ≥ 2 so k-1 ≥ 1 */
    seq_t bin_val = binomial(r, k);      /* 0 ≤ k ≤ r */

    /* Absolute value of the term. */
    seq_t value = fib_val * bin_val;

    /* Apply sign: (-1)^k for k ≥ 2 (positive for even, negative for odd). */
    if (k & 1)                /* odd k -> negative */
        value = -value;

    return value;
}
