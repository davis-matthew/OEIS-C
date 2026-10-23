/*
 * A156992  Triangle T(n,k) = n!*binomial(n-1, k-1) for 1 <= k <= n, read by
 *          rows.
 * https://oeis.org/A156992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the unique integer with * r*(r-1)/2 < n
 *            <= r*(r+1)/2. k = n - (r-1)*r/2 (1‑based index in * row r).
 *            T(r,k) = r! * binomial(r-1, k-1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 58, u64 n <= 158,
 *            u128 n <= 420, big any n
 * Helpers:   binomial, factorial, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    /* find the row r containing index n */
    idx_t tmp = 8 * n + 1;                  /* may overflow only for very large n */
    idx_t s = (idx_t)int_sqrt(tmp);
    idx_t r = (s - 1) / 2;                  /* floor part of the solution */
    /* r may be one too small for values exactly at the boundary */
    if ((r * (r + 1) / 2) < n)
        ++r;

    /* index within row r (1‑based) */
    idx_t rows_before = r * (r - 1) / 2;    /* total elements before row r */
    idx_t k = n - rows_before;              /* 1 <= k <= r */

    /* compute r! * binomial(r-1, k-1) */
    seq_t fact = factorial(r);              /* r! */
    seq_t bin  = binomial(r - 1, k - 1);    /* C(r-1, k-1) */
    return fact * bin;
}
