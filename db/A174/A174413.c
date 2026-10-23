/*
 * A174413  Triangle T(n,m) with the denominator of 1/(n-m)^2-1/n^2, read by
 *          rows, 1<=m<n.
 * https://oeis.org/A174413
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, tabl, look, easy
 * Formula:   T(n,m) = n^2*(n-m)^2 / gcd( m*(2n-m) , n^2*(n-m)^2 ) The
 *            sequence a(k) is read row‑by‑row from the triangle T, with
 *            offset 2 (a(2) is the first term)
 * Range:     u8 n <= 7, u16 n <= 121, u32 n <= 2017, u64 n <= 2017,
 *            u128 n <= 2017, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* offset: a(2) is the first element */
    if (n < 2) return 0;

    /* Convert linear index n to (row r, column m) of the triangle.   */
    uint64_t idx = n - 2;          /* 0–based index into the triangle */
    uint64_t r   = 2;              /* start at row 2 (has 1 element) */
    uint64_t S   = 1;              /* cumulative count up to row r */

    while (S <= idx) {
        r++;
        S = (r - 1) * r / 2;       /* number of terms up to current row */
    }

    uint64_t s_prev = (r - 2) * (r - 1) / 2;  /* terms before row r */
    uint64_t m = idx - s_prev + 1;            /* column (1‑based) */

    /* Compute the triangle entry T(r,m).                           */
    seq_t nn = (seq_t)r;
    seq_t mm = (seq_t)m;

    seq_t diff  = nn - mm;               /* n - m */
    seq_t denom = nn * nn;               /* n^2 */
    denom *= diff * diff;                /* n^2 * (n-m)^2 */

    seq_t num = mm * ((seq_t)2 * nn - mm);  /* m*(2n-m) */

    seq_t g = gcd(num, denom);          /* greatest common divisor */
    return denom / g;
}
