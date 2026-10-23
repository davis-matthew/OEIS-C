/*
 * A105064  Triangle, read by rows, T(n, k) = T(n, k-1) + (k+1)*n!, T(n, 0) =
 *          1.
 * https://oeis.org/A105064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 + n!⋅k⋅(k+3)/2, where r = ⌊(√(8n+1)-1)/2⌋ (row index,
 *            0‑based), k = n - r(r+1)/2 (column index within that row)
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* --- determine row r and column k --- */
    long double disc = sqrtl((long double)8 * (long double)n + 1);
    idx_t r = (idx_t)((disc - 1) / 2);
    /* adjust in case of rounding errors */
    while ((unsigned __int128)r * (r + 1) / 2 > n) r--;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) r++;

    idx_t k = n - (r * (r + 1) / 2);

    /* --- compute factorial n! (here n = r) --- */
    seq_t fact = 1;
    for (idx_t i = 1; i <= r; i++)
        fact *= (seq_t)i;

    /* --- compute the term --- */
    seq_t kk = (seq_t)k;
    seq_t term = kk * (kk + (seq_t)3) / 2;   /* k*(k+3)/2 is always integer */
    return (seq_t)1 + fact * term;
}
