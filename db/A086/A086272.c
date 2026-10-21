/*
 * A086272  Rectangular array T(n,k) of central polygonal numbers, by
 *          antidiagonals.
 * https://oeis.org/A086272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (col+1) * C(row,2) + 1, * where (row,col) are obtained by
 *            enumerating the array * T(k,row) = (k+1)*C(row,2)+1 along
 *            antidiagonals. * Antidiagonals are ordered first (k=1,row=1),
 *            then * (k=1,row=2),(k=2,row=1), then
 *            (k=1,row=3),(k=2,row=2),(k=3,row=1), … * For a given index n
 *            (1‑based) this...
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find diagonal sum s = k + row such that
     * cumulative count of entries up to s-1 is < n
     * and up to s is >= n.  The number of entries on
     * diagonal s is s-1 (since k, row >= 1).
     */
    idx_t s = 2;               // minimal possible sum
    idx_t cumulative = 0;      // entries before current diagonal

    while (cumulative + (s - 1) < n) {
        cumulative += (s - 1);
        ++s;
    }

    /* Offset within the diagonal (0‑based)
     * (k starts at 1, row at s-1).
     */
    idx_t offset = n - cumulative - 1;

    idx_t k     = offset + 1;               // column index (k >= 1)
    idx_t row   = (s - 1) - offset;         // row index (row >= 1)

    /* Compute binomial(row, 2) = row*(row-1)/2 using seq_t */
    seq_t binom = (seq_t)row * (row - 1) / 2;

    /* Final value: (k+1) * binom + 1 */
    return (seq_t)(k + 1) * binom + 1;
}
