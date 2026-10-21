/*
 * A082038  A square array of quadratic-factorial numbers, read by
 *          antidiagonals.
 * https://oeis.org/A082038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the (j,i) entry of the square array
 *          T(j,i) = ((i*j)^2 + i*j + 1) * j!   (columns = j, rows = i),
 *          read by antidiagonals (i+j = const).  The mapping from
 *          index n to coordinates is:
 *              find s such that s*(s+1)/2 ≤ n < (s+1)*(s+2)/2
 *              pref = s*(s+1)/2
 *              off  = n - pref
 *              i = s - off       (row index)
 *              j = off           (column index)
 *          Then a(n) = ((i*j)^2 + i*j + 1) * j!.
 */
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* locate diagonal s */
    idx_t s = 0;
    while ((s + 1) * (s + 2) / 2 <= n)
        ++s;

    /* number of elements before this diagonal */
    idx_t pref = s * (s + 1) / 2;

    /* offset inside the diagonal */
    idx_t off = n - pref;

    /* coordinates (i,j) with i decreasing along the diagonal */
    idx_t i = s - off;   /* row index */
    idx_t j = off;       /* column index */

    /* compute T(j,i) = ((i*j)^2 + i*j + 1) * j! */
    seq_t is = (seq_t)i;
    seq_t js = (seq_t)j;
    seq_t prod = is * js;                 /* i*j */
    seq_t val  = prod * prod + prod + 1;  /* (i*j)^2 + i*j + 1 */
    seq_t fact = factorial(j);            /* j! */

    return val * fact;
}
