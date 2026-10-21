/*
 * A099394  Triangle T(k,n) by rows: n! * A075499(k,n).
 * https://oeis.org/A099394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Stirling2(r+1,c+1) * (c!)*4^(r-c), where r =
 *            floor((sqrt(8*n+1)-1)/2) (row index) c = n - r*(r+1)/2 (column
 *            index)
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the nth term of OEIS A099394 (flattened triangle T) */
seq_t a(idx_t n)
{
    /* Find the row 'r' such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t prev_terms = r * (r + 1) / 2;
    idx_t c = n - prev_terms;           /* column index  (0 <= c <= r) */

    /* Compute Stirling numbers of the second kind S(row+1, c+1)
       using a 1‑D DP array.  Size is small (r <= ~140 for n <= 10000). */
    idx_t sz = r + 2;                    /* We need indices up to r+1 */
    seq_t *S = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!S) return 0;                    /* out of memory – should not happen */

    S[0] = (seq_t)1;
    for (idx_t i = 1; i <= r + 1; ++i) {
        for (idx_t k = i; k >= 1; --k) {
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }
        S[0] = 0;                         /* for the next iteration */
    }
    seq_t stirling = S[c + 1];
    free(S);

    /* factorial of c: c! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= c; ++i)
        fact *= (seq_t)i;

    /* 4^(r-c) */
    seq_t pow4 = (seq_t)1;
    idx_t exp = r - c;
    for (idx_t i = 0; i < exp; ++i)
        pow4 *= (seq_t)4;

    return stirling * fact * pow4;
}
