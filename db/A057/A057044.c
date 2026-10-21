/*
 * A057044  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057044(n)=j(L(n)), where L(n) is
 *          the n-th Lucas number.
 * https://oeis.org/A057044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j(Lucas(n-1))
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   fib_pair, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* j(k) is the column index of number k in the rectangle with antidiagonals 1;2,3;4,5,6;… */

#include <stdbool.h>
#include "../utils/fib_pair.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute the (n-1)-th Lucas number by using Fibonacci numbers.
       Lucas(m) = 3*F(m+1) - F(m+2)  (here m = n-1, so we need F(n) and F(n+1)) */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);           /* fn  = F(n),   fn1 = F(n+1) */

    seq_t lucas = 3 * fn - fn1;       /* Lucas(n-1) */

    /* Find diagonal d and column j such that number lucas lies in that cell.
       The diagonal index d satisfies d*(d+1)/2 >= lucas and (d-1)*d/2 < lucas. */
    seq_t s   = lucas;
    seq_t t   = s * 8 + 1;            /* 8*lucas + 1  */
    seq_t sqrtT = floor_sqrt(t);      /* floor(sqrt(8*lucas+1)) */
    seq_t d   = (sqrtT - 1) / 2;      /* floor((sqrt(8*lucas+1)-1)/2) */

    seq_t mid = d * (d + 1) / 2;
    if (mid < s)                      /* adjust if floor underestimates diagonal */
        d = d + 1;

    seq_t prev = (d - 1) * d / 2;     /* sum up to previous diagonal */
    seq_t offset = s - prev;          /* position within diagonal (1‑based) */
    seq_t j = d - offset + 1;         /* column index */

    return j;
}
