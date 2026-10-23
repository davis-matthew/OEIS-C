/*
 * A186865  Number of 6-step king's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A186865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14576*n^2 - 77924*n + 99292 for n > 4; a(1) = 0, a(2) =
 *            0, a(3) = 2240, a(4) = 22672
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle special cases for small n. */
    if (n <= 4) {
        switch (n) {
            case 1: return 0;
            case 2: return 0;
            case 3: return 2240;
            case 4: return 22672;
            default: return 0; /* unreachable, but keep compiler happy */
        }
    }

    /* For n > 4 use the quadratic formula.  Compute
     * (14576*n^2 + 99292) - 77924*n to avoid unsigned underflow. */
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)14576 * nn * nn + (seq_t)99292;
    seq_t term2 = (seq_t)77924 * nn;
    return term1 - term2;
}
