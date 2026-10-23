/*
 * A156647  Square array T(n, k) = Product_{j=1..n} (1 - ChebyshevT(j, k+1)^2)
 *          with T(n, 0) = n!, read by antidiagonals.
 * https://oeis.org/A156647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = read the square array T(n,k) by antidiagonals, where
 *            T(i,j) = Product_{t=1..i} (1 - ChebyshevT(t, j+1)^2) and T(i,0)
 *            = i!
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 24, i64 n <= 32, i128 n <= 51,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the anti‑diagonal s such that
       s*(s+1)/2 <= n < (s+1)*(s+2)/2.   */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* Transform the index into a row (i) and column (j). */
    idx_t diag_start = s * (s + 1) / 2;
    idx_t offset = n - diag_start;
    idx_t i = offset;          /* row index */
    idx_t j = s - offset;      /* column index */

    /* Column 0 is defined as i!. */
    if (j == 0)
        return factorial(i);

    /* Compute the product over t = 1 .. i of (1 - T_t(j+1)^2).  */
    seq_t prod = 1;
    seq_t x = (seq_t)(j + 1);   /* argument for the Chebyshev polynomials */

    seq_t T_prev = 1;          /* T_0(x) */
    seq_t T_cur  = x;          /* T_1(x) */

    for (idx_t t = 1; t <= i; ++t)
    {
        seq_t val = T_cur;                /* T_t(x) */
        seq_t term = 1 - val * val;
        prod = prod * term;

        /* Prepare the next Chebyshev value for the following iteration. */
        seq_t T_next = 2 * x * T_cur - T_prev;
        T_prev = T_cur;
        T_cur  = T_next;
    }

    return prod;
}
