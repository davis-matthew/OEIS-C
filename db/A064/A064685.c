/*
 * A064685  Length of orbit of 2n+1 in the 3x+1 problem.
 * https://oeis.org/A064685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = length of the Collatz orbit of 2n+1, counting all terms
 *            from 2n+1 through the terminating 1
 * Range:     u8 n <= 12, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)2 * n + 1;   /* start value 2n+1 */
    seq_t steps = 1;              /* count the starting value */

    while (x != 1) {
        if (x & 1) {              /* odd */
            x = 3 * x + 1;
        } else {                  /* even */
            x >>= 1;              /* divide by 2 */
        }
        steps++;
    }

    return steps;
}
