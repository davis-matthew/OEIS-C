/*
 * A027465  Cube of lower triangular normalized binomial matrix.
 * https://oeis.org/A027465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = binomial(r, k) * 3^(r-k), where r = smallest integer such
 *            that (r+1)(r+2)/2 > n k = n - r*(r+1)/2. This gives the n-th
 *            element (0‑based) in the row‑major enumeration of the triangle
 *            with entries T(r,k) = binomial(r,k) * 3^(r-k)
 * Range:     u8 n <= 15, u16 n <= 46, u32 n <= 172, u64 n <= 599,
 *            u128 n <= 2223, big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    idx_t r = 0;          /* current row index   */
    idx_t cum = 0;        /* sum of previous rows */

    /* find the row containing the n-th element */
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        r++;
    }

    idx_t k = n - cum;    /* column within row r */

    seq_t bin = binomial(r, k);   /* C(r,k) */
    seq_t p3  = pow3(r - k);      /* 3^(r-k) */

    return bin * p3;                     /* T(r,k) */
}
