/*
 * A179806  Irregular triangle by rows, M*Q. M = triangle with (1,2,3,...) in
 *          every column, with the leftmost column shifted upwards one row. Q
 *          = a diagonalized variant of odd-indexed Fibonacci numbers.
 * https://oeis.org/A179806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = M[r][c] * F_{2c-1} where (r,c) is the position of the
 *            n-th term in the triangular array, M[r][1] = r and M[r][c] = r -
 *            c for c ≥ 2, and F_k is the (k‑th) Fibonacci number with F_1 =
 *            1, F_2 = 1. The triangular array has row r (r ≥ 2) containing r
 *            – 1 terms and row 1 containing a single term
 * Range:     u8 n <= 34, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided Fibonacci function. */
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Handle the (undefined) index 0 as zero. */
    if (n == 0)
        return (seq_t)0;

    /* Find the row r containing the n‑th term.
       Row 1 has 1 term; row r (r ≥ 2) has r – 1 terms. */
    idx_t r = 1;
    idx_t rem = n;                     /* remaining index within the rows being scanned */

    while (1)
    {
        idx_t count = (r == 1 ? 1 : r - 1);   /* terms in row r */
        if (rem <= count)
            break;                           /* n lies in this row */
        rem -= count;                         /* skip this row */
        r += 1;
    }

    idx_t c = rem;                     /* column index within the row (1‑based) */

    /* M[r][c] according to the definition above. */
    seq_t m_val = (c == 1) ? (seq_t)r : (seq_t)(r - c);

    /* Fibonacci number F_{2c-1} (odd index). */
    seq_t fib_val = fib(2 * c - 1);

    return m_val * fib_val;           /* a(n) */
}
