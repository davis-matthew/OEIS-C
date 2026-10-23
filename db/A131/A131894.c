/*
 * A131894  A131843 * A000012.
 * https://oeis.org/A131894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value at position n of the triangle with rows defined by
 *            r>=0: a(r,0) = 5r + 1 a(r,j) = 3r - (j-1) for 1 <= j <= r
 *            Sequence a(n) is obtained by reading the triangle row by row
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r containing position n.
       r = floor((sqrt(8n+1)-1)/2) with corrections for round‑off. */
    double approx = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((approx - 1.0) / 2.0);

    /* Adjust r to satisfy r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    seq_t n_seq = (seq_t)n;
    seq_t total_prev = (seq_t)r * (r + 1) / 2;
    if (total_prev > n_seq) {
        while (total_prev > n_seq) {
            r--;
            total_prev = (seq_t)r * (r + 1) / 2;
        }
    } else {
        while ((seq_t)(r + 1) * (r + 2) / 2 <= n_seq) {
            r++;
        }
    }

    /* Position j within the row (0-indexed). */
    idx_t j = n - r * (r + 1) / 2;

    /* Compute value according to row formula. */
    if (j == 0) {
        return (seq_t)(5 * r + 1);
    } else {
        return (seq_t)(3 * r - (j - 1));
    }
}
