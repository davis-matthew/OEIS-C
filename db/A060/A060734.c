/*
 * A060734  Natural numbers written as a square array ending in last row from
 *          left to right and rightmost column from bottom to top are read by
 *          antidiagonals downwards.
 * https://oeis.org/A060734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (i‑1)^2 + j if j ≤ i * j^2 + 1 – i if j > i * where (i,j)
 *            is the n‑th pair in the antidiagonal enumeration (diagonal *
 *            index d = ⎣(√(8n – 7)+1)/2⎦, offset = n – d(d – 1)/2 – 1, i =
 *            offset+1, * j = d – offset)
 * Range:     u8 n <= 120, u16 n <= 9488, u32 n <= 9488, u64 n <= 9488,
 *            u128 n <= 9488, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1 only; undefined for 0 */

    /* find the diagonal d such that d(d+1)/2 ≥ n */
    idx_t d = (idx_t)((sqrt((double)8 * n - 7.0) + 1.0) / 2.0);

    /* adjust for possible rounding errors */
    while ((unsigned __int128)d * (d + 1) / 2 < n) ++d;
    while (d > 0 && (unsigned __int128)d * (d - 1) / 2 >= n) --d;

    /* offset within the diagonal (zero‑based) */
    idx_t prev = (idx_t)((unsigned __int128)d * (d - 1) / 2);
    idx_t offset = n - prev - 1;

    idx_t i = offset + 1;
    idx_t j = d - offset;

    if (j <= i) {
        seq_t base = (seq_t)(i - 1);
        return base * base + (seq_t)j;
    } else {
        return (seq_t)j * (seq_t)j + (seq_t)1 - (seq_t)i;
    }
}
