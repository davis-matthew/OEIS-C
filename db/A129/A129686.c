/*
 * A129686  Triangle read by rows: row n is 0^(n-3), 1, 0, 1.
 * https://oeis.org/A129686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the element’s position in its row is the last
 *            element (index r-1) or the third‑from‑last (index r-3);
 *            otherwise a(n) = 0. Row r is defined as: r = 1 → [1]; r = 2 →
 *            [0, 1]; r ≥ 3 → [0^(r-3), 1, 0, 1] (total r elements)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that the cumulative sum of lengths
       up to r is at least n.  For n ≥ 1, r satisfies r(r+1)/2 ≥ n. */
    idx_t r   = 1;
    idx_t cum = 1;            /* r*(r+1)/2 for the current r */
    while (cum < n)
    {
        ++r;
        cum += r;             /* add the new row length */
    }

    /* Index of n within row r (0‑based). */
    idx_t prev = cum - r;     /* sum of lengths up to r-1 */
    idx_t pos  = n - prev - 1;/* position in this row */

    /* Determine the value. */
    if (pos == r - 1)                 return (seq_t)1;
    else if (r > 2 && pos == r - 3)    return (seq_t)1;
    else                               return (seq_t)0;
}
