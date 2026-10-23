/*
 * A158948  Triangle read by rows, left border = natural numbers repeated (1,
 *          1, 2, 2, 3, 3, ...); all other columns = (1, 0, 1, 0, 1, 0, ...).
 * https://oeis.org/A158948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = row r, column c (1‑based).
 * If c == 1:   a(n) = ceil(r/2)          # left border, natural numbers repeated twice.
 * Else:        a(n) = ((r + c) % 2 == 0)? 1 : 0   # bits alternate 1,0,1,0,...
 * Row r has length r and starts at linear index start(r) = r(r‑1)/2 + 1.
 * This implementation maps n to (r,c) in O(1) arithmetic and returns the value.
 */

#include <math.h>
#include <stdbool.h>

/* Triangle by rows, left border = natural numbers repeated (1, 1, 2, 2, 3, 3, ...);
 * all other columns = (1, 0, 1, 0, 1, 0, ...).
 */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 >= n.  Use the quadratic formula. */
    idx_t r = (idx_t)((sqrt((double)(8.0 * (long double)n + 1.0)) - 1.0) * 0.5);
    /* The formula may underestimate by 1 due to rounding; correct it. */
    if ((r * (r + 1)) / 2 < n)
        ++r;

    /* Column within the row (1‑based). */
    idx_t start = ((r - 1) * r) / 2 + 1;          /* first index in this row   */
    idx_t c = n - start + 1;                      /* column   */

    /* Compute the triangle entry. */
    if (c == 1)
    {
        /* Left border: ceil(r/2) = (r+1)/2. */
        return (seq_t)((r + 1) / 2);
    }
    else
    {
        /* Remaining columns alternate 1,0 starting with 1 at row r, column = 2. */
        return (seq_t)(((r + c) & 1) == 0 ? 1 : 0);
    }
}
