/*
 * A056023  The positive integers written as a triangle, where row n is
 *          written from right to left if n is odd and otherwise from left to
 *          right.
 * https://oeis.org/A056023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   use the triangular‑row representation. For a given index n, let
 *            r be the smallest row such that r(r+1)/2 ≥ n. Let a = r(r+1)/2 -
 *            r + 1 be the first number in row r and p = n – a be the 0‑based
 *            offset in the row. If r is even, the row is increasing and the
 *            value is a + p; if r is odd the row is de...
 * Range:     u8 n <= 253, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* safety for a < 1 – defined behavior is unspecified */
    if (n == 0) {
        return (seq_t)0;
    }

    /* find the row r with T(r) = r(r+1)/2 ≥ n */
    idx_t r = 1;
    idx_t t = 1;          /* T(r) */
    while (t < n) {
        ++r;
        t += r;           /* now t == T(r) */
    }

    /* first element of row r */
    idx_t a_start = t - r + 1;   /* T(r-1)+1, fits in idx_t */
    idx_t p = n - a_start;       /* offset inside the row 0‑based */

    if ((r & 1ULL) == 0ULL) {    /* even row: increasing */
        return (seq_t)(a_start + p);
    } else {                    /* odd row: decreasing */
        return (seq_t)(a_start + (r - 1 - p));
    }
}
