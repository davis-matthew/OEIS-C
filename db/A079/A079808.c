/*
 * A079808  Consider a triangle in which the 2n-th row contains first 2n
 *          positive integers in decreasing order and the (2n+1)-st row
 *          contains first 2n+1 positive integers in increasing order;
 *          sequence contains concatenation of numbers read upward at a
 *          45-degree angle.
 * https://oeis.org/A079808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the decimal concatenation of the numbers on the diagonal
 *            with row+col = n+1 of the triangle where even rows are
 *            decreasing (r, r-1, …, 1) and odd rows are increasing (1, 2, …,
 *            r)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 45,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t start = (n + 2) / 2;          /* ceil((n+1)/2) */
    for (idx_t r = n; r >= start; --r) {
        idx_t val;
        if (r & 1U) {                  /* odd row: increasing */
            val = n + 1 - r;            /* value = column index */
        } else {                       /* even row: decreasing */
            val = 2 * r - n;            /* value = r - (n+1-r) + 1 = 2r-n */
        }
        idx_t d = digits(val);          /* number of decimal digits of val */
        seq_t mult = pow10(d);
        result = result * mult + val;
        if (r == start) break;          /* avoid unsigned wrap‑around */
    }
    return result;
}
