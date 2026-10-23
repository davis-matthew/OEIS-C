/*
 * A159746  If an array is made of columns of -nacci sequences, fibo-, tribo-
 *          etc. all starting w. 1,1,2 etc, the NW to SE diagonals can be
 *          extended by computation. The above is diagonal 9. See A159741 for
 *          details.
 * https://oeis.org/A159746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(7+n) - 256 for n >= 5 a(1) = 55, a(2) = 274, a(3) =
 *            773, a(4) = 1793 The sequence is defined for n >= 1; values for
 *            n < 1 are not guaranteed but are ignored
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Handle the small cases explicitly. */
    if (n == 1) return 55;
    if (n == 2) return 274;
    if (n == 3) return 773;
    if (n == 4) return 1793;

    /* For n >= 5 use a(n) = 2^(7+n) - 256. */
    seq_t v = pow2(7 + n);   /* 2^(7+n) */
    v -= (seq_t)256;         /* subtract 256 */
    return v;
}
