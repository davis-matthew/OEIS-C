/*
 * A144693  Triangle read by rows, A000012 * (3*A144328 - 2*A000012), where
 *          A000012 means a lower triangular matrix of all 1's.
 * https://oeis.org/A144693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where r = smallest integer with r(r+1)/2 ≥ n, i.e.
 *            r = ceil((sqrt(1+8n)-1)/2) c = n - r(r-1)/2 (1‑based column in
 *            row r) T(r,c) = (3*c - 5 + 3*[c==1]) * (r-c+1) This reads the
 *            triangle defined in the OEIS entry for A144693
 * Range:     u8 n <= 198, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for n == 0 return 0 (unused). */
    if (n == 0)
        return (seq_t)0;

    /* Find the row r such that r(r+1)/2 ≥ n.  */
    uint64_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < (unsigned __int128)n)
        ++r;

    /* Compute the start index of this row: start = (r-1)r/2.  */
    uint64_t start = (unsigned __int128)(r - 1) * r / 2;

    /* Column (1‑based) within the row.  */
    uint64_t c = n - start;

    /* Compute the triangle entry.  */
    uint64_t coeff = 3 * c - 5 + 3 * (c == 1 ? 1 : 0);
    uint64_t mult  = r - c + 1;

    return (seq_t)coeff * (seq_t)mult;
}
