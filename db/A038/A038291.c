/*
 * A038291  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*1^j.
 * https://oeis.org/A038291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r,k) * 9^(r-k) * where r =
 *            floor((sqrt(8*n+1)-1)/2) (row index) * k = n - r*(r+1)/2 (column
 *            index in that row)
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 65, u64 n <= 210, u128 n <= 820,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute a(n) for OEIS A038291 */
seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    uint64_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        r++;

    /* column index */
    uint64_t prev = r * (r + 1) / 2;
    uint64_t k = n - prev;

    /* compute binomial(r,k) multiplicatively */
    seq_t bin = 1;
    /* use symmetry to minimize multiplications */
    uint64_t m = k;
    if (r - m < m)
        m = r - m;
    for (uint64_t i = 1; i <= m; ++i) {
        bin = bin * (seq_t)(r - m + i) / (seq_t)i;
    }

    /* compute 9^(r-k) */
    seq_t pow9 = 1;
    uint64_t exp = r - k;
    for (uint64_t i = 0; i < exp; ++i)
        pow9 *= (seq_t)9;

    return bin * pow9;
}
