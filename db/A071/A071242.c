/*
 * A071242  Arithmetic mean of n and R(n) where n is a number such that the
 *          least significant digit and the most significant digits are of
 *          same parity and R(n) is its digit reversal (A004086).
 * https://oeis.org/A071242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the arithmetic mean of the nth integer m (counting from
 *            0) whose most significant digit and least significant digit have
 *            the same parity and of m itself. a(n) = (m + R(m))/2, where R(m)
 *            is the decimal digit reversal
 * Range:     u8 n <= 55, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   msd, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/msd.h"
#include "../utils/rev.h"

seq_t a(idx_t n)
{
    idx_t count = 0;          /* how many valid numbers have been seen */
    idx_t m = 0;              /* current integer being tested */

    while (1) {
        /* check parity condition */
        if ( ((msd(m) & 1) == (m % 10 & 1)) ) {
            if (count == n) {
                /* found the nth valid integer */
                seq_t mm  = (seq_t)m;
                seq_t rr  = (seq_t)rev(m);
                return (mm + rr) / 2;
            }
            ++count;
        }
        ++m;
    }
}
