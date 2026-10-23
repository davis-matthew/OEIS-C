/*
 * A155725  Symmetrical product form polynomial as triangle sequence
 *          coefficients: q(x,n)=Product[x + 2*n - i + 1, {i, 0, n - 1}];
 *          p(x,n)=q(x,n)+x^2+x^n*q(1/x,m);t(n,m)=coefficients(p(x,n).
 * https://oeis.org/A155725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   a(n) is the n-th term of the flattened triangle t(r,m), where
 *            for each row r the numbers a_i = 2*r - i + 1 (i = 0..r‑1) form
 *            a_i = r+2, r+3, … , 2r+1. The k‑th elementary symmetric sum e_k
 *            of these numbers satisfies t(r,m) = e_m + e_{r-m}. The sequence
 *            a(n) is obtained by concatenating the rows t...
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    uint64_t nn = (uint64_t)n;
    uint64_t r;
    {
        long double disc = sqrtl((long double)(8.0L * nn + 1.0L));
        r = (uint64_t)((disc - 1.0L) / 2.0L);
        /* correct rounding errors */
        while ((r + 1) * (r + 2) / 2 <= nn) ++r;
        while (r * (r + 1) / 2 > nn) --r;
    }

    uint64_t start = r * (r + 1) / 2;
    uint64_t m = nn - start;  /* position in row, 0‑based */

    /* Compute elementary symmetric sums e[0..r] for the set
       a_i = 2*r - i + 1. */
    seq_t *e = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!e) return (seq_t)0;          /* allocation failure fallback */
    for (uint64_t i = 0; i <= r; ++i) e[i] = 0;
    e[0] = 1;                         /* e_0 = 1 */

    for (uint64_t i = 0; i < r; ++i) {
        seq_t ai = (seq_t)(2 * r - i + 1);
        for (int k = (int)i + 1; k >= 1; --k) {
            e[k] = e[k] + ai * e[k - 1];
        }
    }

    seq_t result = e[m] + e[r - m];
    free(e);
    return result;
}
