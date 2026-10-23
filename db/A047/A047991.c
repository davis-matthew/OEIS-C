/*
 * A047991  Triangle formed from expansion of (x-1)*(x+2)*(x-3)*...*(x+-n).
 * https://oeis.org/A047991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy, nice
 * Formula:   a(n) is the n‑th element of the triangle formed from the
 *            expansion of (x-1)(x+2)(x-3)…(x+(-1)^(i+1)(i+1)). The triangle
 *            is flattened row‑wise (row‑major order). The recurrence for the
 *            triangle is: a(n,0) = 1 a(n,k) = a(n-1,k) + s * n * a(n-1,k-1)
 *            (1 ≤ k ≤ n) where s = +1 if n is even, s = -1 if n...
 * Range:     i8 n <= 24, i16 n <= 43, i32 n <= 101, i64 n <= 248,
 *            i128 n <= 626, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r (0‑based) and column k such that the
       flattened index n lies in row r at position k. */
    idx_t r = 0;
    idx_t cum = 1;                      /* cumulative entries up to row r */
    while (cum <= n) {
        r++;
        cum += r + 1;                   /* add length of next row */
    }
    idx_t cum_prev = cum - (r + 1);      /* entries before row r */
    idx_t k = n - cum_prev;              /* column within row r */

    /* The largest row we ever need for n ≤ 10000 is about 142,
       so a static size of 200 is safely enough. */
    const idx_t MAXROW = 200;
    seq_t row[MAXROW];
    seq_t next[MAXROW];

    /* Initialise the triangle.  For values beyond current row
       we treat them as 0, hence zero the whole array first. */
    for (idx_t i = 0; i < MAXROW; ++i) row[i] = 0;
    row[0] = 1;                         /* row 0 */

    /* Build rows up to r.  Each iteration computes the next row
       from the current one. */
    for (idx_t cur = 1; cur <= r; ++cur) {
        next[0] = 1;
        int sign = (cur % 2 == 0) ? 1 : -1;
        for (idx_t kk = 1; kk <= cur; ++kk) {
            seq_t term = ((seq_t)cur) * row[kk - 1];
            if (sign < 0) term = -term;
            next[kk] = row[kk] + term;
        }
        /* copy the freshly computed row back */
        for (idx_t kk = 0; kk <= cur; ++kk)
            row[kk] = next[kk];
    }

    return row[k];
}
