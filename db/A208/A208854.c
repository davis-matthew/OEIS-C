/*
 * A208854  Array of odd catheti of primitive Pythagorean triangles when read
 *          by SW-NE diagonals.
 * https://oeis.org/A208854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A208854
   a(n) = value on the n-th entry of the SW‑NE diagonals of the
          array a(n,m) = abs((2*n-1)^2 - (2*m)^2) if gcd(2*n-1,2*m)=1
          otherwise 0.  Indices start at n = 1.  The function uses
          only integer arithmetic and the provided gcd routine. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Return abs((2*n-1)^2 - (2*m)^2) if gcd(2*n-1,2*m)=1, else 0. */
static seq_t cathetus_value(idx_t n_prime, idx_t m)
{
    seq_t u = (seq_t)n_prime * 2ULL - 1ULL;   /* u = 2*n' - 1 (odd) */
    seq_t v = (seq_t)m * 2ULL;                /* v = 2*m     (even) */

    if (gcd(u, v) != (seq_t)1ULL)
        return 0;

    /* Compute |u^2 - v^2| */
    if (u >= v)
        return (u * u) - (v * v);
    else
        return (v * v) - (u * u);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* Undefined for n < 1 */

    idx_t k = n;
    idx_t row = 1;
    /* Find the row such that 1 + 2 + ... + row  >= k */
    while (row * (row + 1) / 2 < k)
        ++row;

    idx_t prev_tri = (row - 1) * row / 2;          /* (row-1)*row/2 */
    idx_t m = k - prev_tri;                       /* column index, 1-based */
    idx_t n_prime = row - m + 1;                  /* n' for a(n',m) */

    return cathetus_value(n_prime, m);
}
