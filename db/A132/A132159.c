/*
 * A132159  Lower triangular matrix T(n,j) for double application of an
 *          iterated mixed order Laguerre transform inverse to A132014.
 *          Coefficients of Laguerre polynomials (-1)^n * n! * L(n,-2-n,x).
 * https://oeis.org/A132159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 77, u64 n <= 209,
 *            u128 n <= 594, big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) reads the lower‑triangular matrix T(n,k) row by row.
 * T(r,c) = binomial(r,c) * (r‑c+1)! ,
 * with r = floor((sqrt(8n+1)-1)/2),  c = n - r(r+1)/2 .
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the row r containing position n.  Since n ≤ 10000 on the test
       suite a simple O(n) scan is fast enough and keeps the code short. */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) r++;

    /* Column index within the row */
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;          /* 0 ≤ c ≤ r */

    /* Compute the matrix entry: binomial(r,c) * (r‑c+1)! */
    seq_t bin = binom(r, c);
    seq_t fact = factorial(r - c + 1);

    return bin * fact;
}
