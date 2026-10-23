/*
 * A142457  A triangular sequence "representation" of the modulo 11 Integer
 *          field: t(+)(n,m)=Mod[n + m, 11]; t(x)(n,m)=Mod[n*m, 11];
 *          t(n,m)=Mod[t(=)(n,m)*t(X)(n,m),11].
 * https://oeis.org/A142457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) is the entry at position n in the triangular array defined
 *            by t(n,m) = ((n+m) mod 11) * ((n*m) mod 11) mod 11, where n and
 *            m are 0‑based row and column indices
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Converting linear index n−1 to (row, col):
   r = largest integer with r(r+1)/2 ≤ n−1
   c = (n−1) − r(r+1)/2
   The function uses bit‑wise arithmetic only for the final mod 11
   computation. */
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; flattening starts at n=1 */
    idx_t idx = n - 1;          /* 0‑based linear position */

    /* Find row r via the inverse of triangular numbers.
       Use a double approximation and correct by adjustment. */
    double approx = (sqrt(8.0 * idx + 1.0) - 1.0) / 2.0;
    idx_t r = (idx_t)floor(approx);

    /* Make sure r satisfies r(r+1)/2 ≤ idx < (r+1)(r+2)/2 */
    while ((r + 1) * (r + 2) / 2 <= idx) r++;
    while (r * (r + 1) / 2 > idx) r--;

    idx_t c = idx - r * (r + 1) / 2;   /* column index */

    /* Compute modulo 11 components. */
    idx_t sum_mod = (r + c) % 11;
    idx_t prod_mod = (r * c) % 11;

    /* Final value in the triangular sequence. */
    seq_t res = (seq_t)((sum_mod * prod_mod) % 11);
    return res;
}
