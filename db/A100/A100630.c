/*
 * A100630  Array read by antidiagonals: T(m,n) = Sum_{i=1..m} i*(n-1+i)!.
 * https://oeis.org/A100630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the triangular array T(m,n) read by antidiagonals.
 *            T(m,n) = Σ_{i=1..m} i·(n-1+i)! for m,n ≥ 1. The antidiagonal
 *            index d = m+n runs from 2 upward. For each d, terms are listed
 *            in order of increasing m (m=1..d-1). Thus a(n) is obtained by
 *            finding the smallest d with (d-1)d/2 ≥ n, then setti...
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 74, u64 n <= 197,
 *            u128 n <= 562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t N)
{
    /* find diagonal d (with m+n=d) containing the nth element */
    idx_t d = 2;
    idx_t cumulative = 1;           /* terms in diagonals up to d */
    while (cumulative < N) {
        ++d;
        cumulative += d - 1;
    }

    /* position within the diagonal */
    idx_t m = N - ((d - 2) * (d - 1) / 2);   /* 1 ≤ m ≤ d-1 */
    idx_t nn = d - m;                         /* corresponding n */

    /* compute the sum Σ_{i=1..m} i·(nn-1+i)! efficiently */
    seq_t sum = 0;
    seq_t factorial = 1;       /* holds k! during the loop over k */

    /* k runs from 1 to d-1; the factorial needed is (nn-1+i)! = k!   */
    for (idx_t k = 1; k < d; ++k) {
        factorial *= (seq_t)k;           /* compute k! */

        if (k >= nn) {                  /* k = nn-1+i  →  i = k-(nn-1) */
            idx_t i = k - (nn - 1);
            sum += (seq_t)i * factorial;
        }
    }
    return sum;
}
