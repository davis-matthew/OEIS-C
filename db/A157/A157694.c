/*
 * A157694  Triangle read by rows: the Pascal triangle A007318 with all
 *          elements replaced by 1 which do not equal the central binomial
 *          coefficients.
 * https://oeis.org/A157694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 1 for all indices n except when n corresponds to a *
 *            central binomial coefficient of some row r of Pascal's triangle.
 *            * In that case a(n) = binomial(r, ⌊r/2⌋). The triangle is
 *            indexed * linearly starting with a(0) = 1, a(1) = 1, a(2) = 1,
 *            a(3) = 1, … as * described in the OEIS entry
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Include the binomial routine supplied by the build system.  */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r and the position pos within that row that
     * contain the linear index n.  The triangle has row lengths
     * 1, 2, 3, …, so we walk down rows until the cumulative
     * length exceeds n. */
    idx_t r = 0;           /* current row number (0‑based) */
    idx_t offset = 0;      /* linear index of the first element of row r */

    while (n >= offset + r + 1)
    {
        offset += r + 1;   /* move to the start of the next row */
        r++;              /* advance to next row number */
    }

    idx_t pos = n - offset;   /* position within row r, 0‑based */

    /* The entry is the central binomial coefficient only if it is
     * one of the central positions.  For an even row r the single
     * central position is r/2.  For an odd row the two central
     * positions are r/2 and r/2+1.  In all other cases the entry
     * is replaced by 1. */
    if (pos == r / 2 || ((r & 1) && pos == r / 2 + 1))
    {
        return binomial(r, r / 2);   /* central value */
    }
    return (seq_t)1;                 /* all other entries are 1 */
}
