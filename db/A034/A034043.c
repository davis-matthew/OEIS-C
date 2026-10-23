/*
 * A034043  Numbers that are imprimitively represented by x^2+y^2+z^2.
 * https://oeis.org/A034043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   has_nontrivial_square, sum_of_three_squares
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A034043: Numbers that are sum of three squares and are not squarefree
   (i.e., they have a square divisor >1). This function finds the nth term
   of that sequence in increasing order (a(1)=0). */

#include <stdbool.h>
#include "../utils/has_nontrivial_square.h"
#include "../utils/sum_of_three_squares.h"

/* sequence entry a(n), 1‑based offset */
seq_t a(idx_t n)
{
    if (n == 1) return 0;              /* a(1) = 0 */

    idx_t count = 1;                   /* we already counted 0 */
    for (seq_t m = 1; ; ++m)
    {
        if (sum_of_three_squares(m) && has_nontrivial_square(m))
            ++count;
        if (count == n)
            return m;
    }
}
