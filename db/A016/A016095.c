/*
 * A016095  Triangular array T(n,k) read by rows, where T(n,k) = coefficient
 *          of x^n*y^k in 1/(1-x-y-(x+y)^2).
 * https://oeis.org/A016095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th entry of the flattened triangle T(r,k) =
 *            Fib(r+1) * C(r,k) read row by row. Row r is found by locating r
 *            such that (r+1)(r+2)/2 > n. Index within the row k = n -
 *            r(r+1)/2
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/fib.h"

/* external linkage as required */
seq_t a(idx_t n)
{
    /* find the row r where n lies: minimal r with (r+1)(r+2)/2 > n */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* start index of row r */
    idx_t start = r * (r + 1) / 2;
    /* offset within the row */
    idx_t k = n - start;

    seq_t f  = fib(r + 1);        /* Fib(r+1) */
    seq_t c  = binomial(r, k);    /* C(r,k) */
    return f * c;
}
